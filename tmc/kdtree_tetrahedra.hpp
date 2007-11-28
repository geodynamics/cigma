#ifndef GTB_KDTET_INCLUDED
#define GTB_KDTET_INCLUDED

#include "Bounding_box_3d.hpp"
#include "Point_3f.hpp"
#include "Indexed_tetra_set.hpp"
#include "common.hpp"

#ifndef GTB_C_ASSERT_INCLUDED
#include <assert.h>
#define GTB_C_ASSERT_INCLUDED
#endif

#include <vector>
#include <queue>
#include "Point_3d.hpp"
#include "Point_3f.hpp"
#include "Vector_3d.hpp"
#include "ptrs.h"

#ifdef WIN32
#define random rand
#endif

GTB_BEGIN_NAMESPACE

struct kdtree_exception_no_more_values {};
extern kdtree_exception_no_more_values _exception_no_more_values;

#define KDT_INFINIT_NEIGHBORS 0x7fffffff

/*---------------------- Helper functions --------------*/
void SplitBBox(
	const Bounding_box_3d& bbox,      // Input bounding box
	int axis,
	double position,        // axis, position define where to split
	Bounding_box_3d& lbbox, 
	Bounding_box_3d& rbbox            // Result is store in [lr]bbox
	);
/*---------------------- [ Helper functions ] --------------*/

//
// template arguments:
//    T - the type of object that is stored in the tree
//
template<class T>
class KDTet
{
public:
	typedef unsigned int size_type;
	typedef std::vector<T> result_sequence;

	KDTet(float MAX_TET_PER_CELL, Indexed_tetra_set *its, Scalar_time_series *sts);
	KDTet(float MAX_TET_PER_CELL, Indexed_tetra_set *its, Vector_time_series *sts);
	~KDTet()
		{
			delete root;
		}

	const Bounding_box_3d& GetBBox() const { return bbox; }

	void MakeTree();

	/*--------------------- tree data structures -----------------*/

	struct Node
	{
		Node() {}
		virtual ~Node() {}

		enum node_type {leaf, tree, single}; // Types of nodes

		virtual node_type Type() const=0;
	};

	struct TreeNode : public Node
	{
		~TreeNode()
			{
				delete l;
				delete r;
			}

		typename Node::node_type Type() const { return Node::tree; }

		// Which axis is cut by this node
		// Numbering complies with VX, VY, VZ defined in algebra3.h
		enum t_axis {XAxis=0, YAxis, ZAxis};

		double cut_point;       // where along the axis is this node cut.
		// Absolute coordinates.

		Node *l, *r;            // Left and right sons
		t_axis axis;
	}; // TreeNode

	struct LeafNode : public Node
	{
		LeafNode() {}
		typename Node::node_type Type() const { return Node::leaf; }
        
		void Insert(T v) {
			objects.push_back(v);
		}

		template<class IT>
		void Insert(IT first, IT last)
			{		
				objects.insert(objects.end(), first, last);
			}

		template<class IT>
		void Remove(IT first, IT last) {
			objects.erase(first, last);
		}

		//
		// Reserve space for additional K items, return previous # of elements
		//
		int Resize(int K)
			{
				int n = objects.size();
				objects.resize(n+K);
				return n;
			}

		typedef std::vector<T> t_objects_list;
		t_objects_list objects; // List of objects in the node
    
		typename t_objects_list::value_type& operator[](int n) {
			return objects[n];
		}

	}; // LeafNode

	struct SingleObject : public Node
	{
		T* object;
		explicit SingleObject() : Node() {}
		SingleObject(T* r_object) : Node(), object(r_object) {}
		SingleObject(const SingleObject& rhs) : Node(), object(rhs.object) {}

		void set(T* r_object) { object = r_object; }
		typename Node::node_type Type() const { return Node::single; }
	};


	LeafNode* Find(const Point_3d& p, TreeNode*& parent) const;
	bool FindAndEvalTet(LeafNode *node, const Point_3f& p, float *ret) const;
	bool FindAndEvalTet(LeafNode *node, const Point_3f& p, Vector_3f *ret) const;
	void FindAndEvalTetNN(LeafNode *node, const Point_3f& p, float *ret) const;
	void FindAndEvalTetAvg3N(LeafNode *node, const Point_3f& p, float *ret) const;

protected:
	//
	// Search for the leaf node that contains p
	//
	// Return:
	//   bbox - for the leaf node containing p
	//   parent
	//
	LeafNode* Find(const Point_3d& p, Bounding_box_3d& bbox, TreeNode*& parent) const;

	void MakeTree(TreeNode* root);

	//
	// Split a leaf node, returning a pointer to the
	// parent of the node that holds a new subtree
	//
	Node* Split (
		LeafNode* cell         // Input cell to split
		);

private:
	void tree_depth(Node* root_, int depth, int& max_depth, int& sum_depths, int& num_leaves, int& num_objects, int& num_nodes) const;

	float max_tet_per_cell;
	Indexed_tetra_set *its;
	Scalar_time_series *sts;
    Vector_time_series *vts;
	Bounding_box_3d bbox;
        Node* root;

}; // KDTet


template<class T>
inline KDTet<T>::KDTet(float MAX_TET_PER_CELL, Indexed_tetra_set *in_its, Scalar_time_series *in_sts) :
	max_tet_per_cell(MAX_TET_PER_CELL),
	its(in_its),
	sts(in_sts),
	bbox(in_its->get_bounding_box()),
	root(new LeafNode)
{
	unsigned num_tets = in_its->get_tetrahedra().size();
	for (unsigned ti = 0; ti < num_tets; ti++) {
		LeafNode* node = static_cast<LeafNode*>(root);
		node->Insert(ti);
	}
}

template<class T>
inline KDTet<T>::KDTet(float MAX_TET_PER_CELL, Indexed_tetra_set *in_its, Vector_time_series *in_vts) :
	max_tet_per_cell(MAX_TET_PER_CELL),
	its(in_its),
	vts(in_vts),
	bbox(in_its->get_bounding_box()),
	root(new LeafNode)
{
	unsigned num_tets = in_its->get_tetrahedra().size();
	for (unsigned ti = 0; ti < num_tets; ti++) {
		LeafNode* node = static_cast<LeafNode*>(root);
		node->Insert(ti);
	}
}

template<class T>
void KDTet<T>::MakeTree()
{
	if (root->Type() == Node::leaf)
	{
		root = Split(static_cast<LeafNode*>(root));
	}
	else
	{
		MakeTree(static_cast<TreeNode*>(root));
	}
}

template<class T>
void KDTet<T>::MakeTree(TreeNode* arg_root)
{
	if (arg_root->l->Type() == Node::leaf)
	{
		arg_root->l = Split(static_cast<LeafNode*>(arg_root->l));
	}
	else
	{
		MakeTree(static_cast<TreeNode*>(arg_root->l));
	}

	if (arg_root->r->Type() == Node::leaf)
	{
		arg_root->r = Split(static_cast<LeafNode*>(arg_root->r));
	}
	else
	{
		MakeTree(static_cast<TreeNode*>(arg_root->r));
	}
}

template<class T>
bool  KDTet<T>::FindAndEvalTet(LeafNode *node, const Point_3f& p, float *ret) const
{
	typename LeafNode::t_objects_list::iterator f = node->objects.begin();
	typename LeafNode::t_objects_list::iterator l = node->objects.end();
	const std::vector<Tetrahedron> &tets = its->get_tetrahedra();
	for (; f != l; ++f)
	{
		const Tetrahedron &t = tets[*f];
		if (!its->approx_inside_tetra(t, p)) continue;
		float value;
		if (its->interpol_tetra(sts, t, p, &value)) {
			*ret = value;
			return true;
		}
	}
	return false;
}

template<class T>
bool  KDTet<T>::FindAndEvalTet(LeafNode *node, const Point_3f& p, Vector_3f *ret) const
{
	typename LeafNode::t_objects_list::iterator f = node->objects.begin();
	typename LeafNode::t_objects_list::iterator l = node->objects.end();
	const std::vector<Tetrahedron> &tets = its->get_tetrahedra();
	for (; f != l; ++f)
	{
		const Tetrahedron &t = tets[*f];
		if (!its->approx_inside_tetra(t, p)) continue;
		Vector_3f value;
		if (its->interpol_tetra(vts, t, p, &value)) {
			*ret = value;
			return true;
		}
	}
	return false;
}


template<class T>
void KDTet<T>::FindAndEvalTetAvg3N(LeafNode *node, const Point_3f& p, float *ret) const
{
	typename LeafNode::t_objects_list::iterator f = node->objects.begin();
	typename LeafNode::t_objects_list::iterator l = node->objects.end();
	const std::vector<Tetrahedron> &tets = its->get_tetrahedra();
	const std::vector<Point_3f> &verts = its->get_vertices();

	float min_distance = FLT_MAX;
	float nearest_values[3] = {0, 0, 0};

	for (; f != l; ++f)
	{
		const Tetrahedron &t = tets[*f];
		for (unsigned j=0; j<4; j++) {
			unsigned vi = t.get_vertex(j);
			const Point_3f &v = verts[vi];
			float d = Point_3f::distance(v, p);
			if (d < min_distance) {
				min_distance = d;
				nearest_values[2] = nearest_values[1];
				nearest_values[1] = nearest_values[0];
				nearest_values[0] = sts->get_value(vi);
			}
		}
	}
	
	*ret = (nearest_values[0] + nearest_values[1] + nearest_values[2]) / 3;
}


template<class T>
void KDTet<T>::FindAndEvalTetNN(LeafNode *node, const Point_3f& p, float *ret) const
{
	typename LeafNode::t_objects_list::iterator f = node->objects.begin();
	typename LeafNode::t_objects_list::iterator l = node->objects.end();
	const std::vector<Tetrahedron> &tets = its->get_tetrahedra();
	const std::vector<Point_3f> &verts = its->get_vertices();

	float min_distance = FLT_MAX;
	float nearest_value = 0;

	for (; f != l; ++f)
	{
		const Tetrahedron &t = tets[*f];
		for (unsigned j=0; j<4; j++) {
			unsigned vi = t.get_vertex(j);
			const Point_3f &v = verts[vi];
			float d = Point_3f::distance(v, p);
			if (d < min_distance) {
				min_distance = d;
				nearest_value = sts->get_value(vi);
			}
		}
	}
	*ret = nearest_value;
}


template<class T>
typename KDTet<T>::LeafNode* KDTet<T>::Find(const Point_3d& p, typename KDTet<T>::TreeNode*& parent) const
{
	if (root->Type() == Node::leaf)
	{
		parent = 0;
		return static_cast<LeafNode*>(root);
	}
	else
	{
		parent = static_cast<TreeNode*>(root);
		while (1)
		{
			if (parent->cut_point < p[parent->axis])
			{
				// we're going right
				if (parent->r->Type() == Node::leaf)
				{
					return static_cast<LeafNode*>(parent->r);
				}
				parent = static_cast<TreeNode*>(parent->r);
			}
			else
			{
				// we're going left
				if (parent->l->Type() == Node::leaf)
				{
					return static_cast<LeafNode*>(parent->l);
				}
				parent = static_cast<TreeNode*>(parent->l);
			}
		}
	}
}


template<class T>
typename KDTet<T>::LeafNode* KDTet<T>::Find(const Point_3d& p, Bounding_box_3d& leafbbox, typename KDTet<T>::TreeNode*& parent) const
{
	leafbbox = bbox;
	if (root->Type() == Node::leaf)
	{
		parent = 0;
		return static_cast<LeafNode*>(root);
	}
	else
	{
		parent = static_cast<TreeNode*>(root);
		Bounding_box_3d lbbox, rbbox;
		while (1)
		{
			SplitBBox(leafbbox, parent->axis, parent->cut_point, lbbox, rbbox);
			if (parent->cut_point < p[parent->axis])
			{
				// we're going right
				leafbbox = rbbox;
				if (parent->r->Type() == Node::leaf)
				{
					return static_cast<LeafNode*>(parent->r);
				}
				parent = static_cast<TreeNode*>(parent->r);
			}
			else
			{
				// we're going left
				leafbbox = lbbox;
				if (parent->l->Type() == Node::leaf)
				{
					return static_cast<LeafNode*>(parent->l);
				}
				parent = static_cast<TreeNode*>(parent->l);
			}
		}
	}
}

template<class T>
typename KDTet<T>::Node* KDTet<T>::Split (
	typename KDTet<T>::LeafNode* cell  // Input cell to split
	)
{
	unsigned n_objects = cell->objects.size();
	if (n_objects < max_tet_per_cell)
		return cell;

	typename TreeNode::t_axis axis;
	std::vector<Point_3f> &vertices = its->get_vertices();
	const std::vector<Tetrahedron> &tets = its->get_tetrahedra();
//	    const std::vector<Point_3f> &centroids = its->get_centroids();

	double cut_point;
	{
		Point_3f p[3];
		for (unsigned j=0; j<3; j++) {
			unsigned idx = random() % n_objects;
#if 1
			unsigned tid = cell->objects[idx];
			p[j] = Point_3f::centroid(vertices[tets[tid].get_vertex(0)],
						  vertices[tets[tid].get_vertex(1)],
						  vertices[tets[tid].get_vertex(2)],
						  vertices[tets[tid].get_vertex(3)]);
#else
			p[j] = centroids[cell->objects[idx]];
#endif
		}
		Point_3f min_point(
			min3(p[0][0],p[1][0],p[2][0]),
			min3(p[0][1],p[1][1],p[2][1]),
			min3(p[0][2],p[1][2],p[2][2]));
		Point_3f max_point(
			max3(p[0][0],p[1][0],p[2][0]),
			max3(p[0][1],p[1][1],p[2][1]),
			max3(p[0][2],p[1][2],p[2][2]));

		Vector_3f box_dimentions = max_point - min_point;

		int k;
		max3(box_dimentions[0], box_dimentions[1], box_dimentions[2], k);
		axis = static_cast<typename TreeNode::t_axis>(k);
		cut_point = min_point[axis] + box_dimentions[axis]*0.51;
	}

	//
	// Make the split
	//
//	    printf("Splitting at %d %g\n", axis, cut_point);

	LeafNode* rightnode = new LeafNode;

	typename LeafNode::t_objects_list::iterator last_left = cell->objects.end();
	typename LeafNode::t_objects_list::iterator f = cell->objects.begin();
	while (f < last_left)
	{
		const Tetrahedron &t = tets[*f];
		bool is_in_left_cell = false;
		bool is_in_right_cell = false;
		for (unsigned vi=0; vi < 4; vi++)
		{
			Point_3f v = vertices[t.get_vertex(vi)];
			if (v[axis] <= cut_point)
			{
				is_in_left_cell = true;
			}
			else
			{
				is_in_right_cell = true;
			}
		}
		if (is_in_left_cell && !is_in_right_cell) {
			++f;
		} else if (is_in_right_cell && !is_in_left_cell) {
			--last_left;
			rightnode->objects.push_back(*f);
			*f = *last_left;
		} else {
			rightnode->objects.push_back(*f);
			++f;
		}
	}

	if (last_left == cell->objects.end())
	{
		// we have many objects that are equal
		delete rightnode;
		return cell;
	}

	cell->Remove(last_left, cell->objects.end());

	TreeNode* subroot = new TreeNode;
	subroot->cut_point = cut_point;
	subroot->axis = axis;
	subroot->l = Split(cell);
	subroot->r = Split(rightnode);
	return subroot;
}

GTB_END_NAMESPACE

#endif // GTB_KDTET_INCLUDED
