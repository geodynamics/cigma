#ifndef GTB_INDEXED_TETRA_SET_INCLUDED
#define GTB_INDEXED_TETRA_SET_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_PLANE_3F_INCLUDED
#include "Plane_3f.hpp"
#endif

// #ifndef GTB_COLOR_3F_INCLUDED
// #include "Color_3f.hpp"
// #endif

// #ifndef GTB_COLOR_4F_INCLUDED
// #include "Color_4f.hpp"
// #endif

#ifndef GTB_TETRAHEDRON_INCLUDED
#include "Tetrahedron.hpp"
#endif

#ifndef GTB_BOUNDING_BOX_3D_INCLUDED
#include "Bounding_box_3d.hpp"
#endif

#ifndef GTB_SHAPE_INCLUDED
#include "Shape.hpp"
#endif

#ifndef GTB_TIME_SERIES_INCLUDED
#include "Time_series.hpp"
#endif

GTB_BEGIN_NAMESPACE


class Indexed_tetra_set : public Shape {
public:
	Indexed_tetra_set();

	Indexed_tetra_set(const std::vector<Point_3f> &vertices, 
			  const std::vector<Tetrahedron> &tetrahedra);

	Indexed_tetra_set(const std::vector<Point_3f> &vertices, 
			  const std::vector<Tetrahedron> &tetrahedra,
			  bool replicate);

	Indexed_tetra_set(const std::vector<Point_3f> &vertices, 
			  const std::vector<unsigned> &vertex_global_indices,
			  const std::vector<Tetrahedron> &tetrahedra); 

	Indexed_tetra_set(const Indexed_tetra_set &src, 
			  const std::vector<Tetrahedron> &src_tetra); 

	~Indexed_tetra_set();

	void add(const Indexed_tetra_set &src); 

	class Face {
	public:
		Face();
		Face(unsigned i, unsigned j, unsigned k);
		void set_tetra(int t); // tetra 0 is reserved

		void set_vertices(unsigned v0, unsigned v1, unsigned v2);
		unsigned get_vertex(unsigned i) const;

		unsigned get_left_tetra() const;
		void set_left_tetra(unsigned t);

		unsigned get_right_tetra() const;
		void set_right_tetra(unsigned t);

		char get_index_on_left_tetra() const;
		void set_index_on_left_tetra(char i);

		char get_index_on_right_tetra() const;
		void set_index_on_right_tetra(char i);

		unsigned get_left_octree_node() const;
		void set_left_octree_node(unsigned on);

		unsigned get_right_octree_node() const;
		void set_right_octree_node(unsigned on);

		bool is_exterior() const;
		void set_exterior(bool value);

		const Plane_3f &get_plane() const;
		void set_plane(const Plane_3f &p);
		void orient_normal(const Point_3f &p);

		bool operator==(const Face &b) const;
		bool operator<(const Face &b) const;

	protected:
		unsigned m_vertices[3];
		Plane_3f m_plane;
		unsigned m_left_tetra;
		unsigned m_right_tetra;
		char m_index_on_left_tetra;
		char m_index_on_right_tetra;
		unsigned m_left_octree_node;
		unsigned m_right_octree_node;
		bool m_is_exterior;
	};

	// Shape interface
	const char *get_class_name() const;
	static const char *static_get_class_name();
	const Bounding_box_3d &get_bounding_box() const;
	unsigned get_size_in_bytes() const;
	void write(FILE *fp) const;
	static Shape *read(FILE *fp);
// 	void render(Rendering_context &rc);
// 	void render_geometry(Rendering_context &rc);
	void accept(Visitor &visitor);
	void transform(const Matrix_4d &m);

	void export_exterior_faces(FILE *fp);
	void build(unsigned octree_node_id);
	void set_as_interior_face(unsigned face, 
				  unsigned index_on_left_tetra,
				  unsigned right_octree_node,
				  unsigned right_tetra,
				  unsigned index_on_right_tetra);

	const std::vector<Point_3f> &get_vertices() const;
	std::vector<Point_3f> &get_vertices();

	const std::vector<unsigned> &get_vertex_global_indices() const;
	std::vector<unsigned> &get_vertex_global_indices();

	const Tetrahedron &get_tetrahedron(unsigned i) const;

	const std::vector<Tetrahedron> &get_tetrahedra() const;
	std::vector<Tetrahedron> &get_tetrahedra();

	const std::vector<Face> &get_faces() const;
	std::vector<Face> &get_faces();

	const std::vector<Point_3f> &get_centroids() const;
	std::vector<Point_3f> &get_centroids();

	unsigned get_global_vertex(unsigned local_vertex);

	unsigned get_num_vertices() const;
	unsigned get_num_primitives() const;
	unsigned get_num_tetrahedra() const;
	unsigned get_num_faces() const;
	unsigned get_num_centroids() const;
	unsigned get_num_exterior_faces() const;
	unsigned get_num_exterior_tetrahedra() const;
	bool is_complete() const;
	bool is_out_of_core() const;

	unsigned get_id() const;
	void set_id(unsigned id);

	void shift(float z_shif);

	bool approx_inside_tetra(const Tetrahedron &t, const Point_3f &p);
	bool interpol_tetra(Scalar_time_series *ts, const Tetrahedron &t, const Point_3f &p, float *value);
	bool interpol_tetra(Vector_time_series *ts, const Tetrahedron &t, const Point_3f &p, Vector_3f *value); //
	bool eval(Scalar_time_series *ts, const Point_3f &p, float *result);

	void print_status();

protected:

	explicit Indexed_tetra_set(FILE *fp);

	void build_centroids();
	void build_faces();
	void build_tetrahedra();
	void compute_connectivity();
	bool are_tetrahedra_vertices_valid() const;

	std::vector<Point_3f> m_vertices;
	std::vector<Tetrahedron> m_tetrahedra;
	std::vector<Face> m_faces;
	std::vector<Point_3f> m_centroids;
	std::vector<unsigned> m_global_indices;
	Bounding_box_3d m_bounding_box;
	static const char *m_class_name;
	
	unsigned m_id;
	bool m_complete;
	bool m_out_of_core;
	unsigned m_num_exterior_faces;
	unsigned m_num_exterior_tetrahedra;
};


struct Indexed_tetra_set_init {
	Indexed_tetra_set_init();
	~Indexed_tetra_set_init();
};


static Indexed_tetra_set_init indexed_tetra_set_init;


inline unsigned Indexed_tetra_set::get_id() const
{
	return m_id;
}


inline const char *Indexed_tetra_set::get_class_name() const
{
	return m_class_name;
}


inline const char *Indexed_tetra_set::static_get_class_name()
{
	return m_class_name;
}


inline const Bounding_box_3d &Indexed_tetra_set::get_bounding_box() const
{
	return m_bounding_box;
}


inline unsigned Indexed_tetra_set::get_size_in_bytes() const
{
        return sizeof(m_num_exterior_faces)
		+ sizeof(m_num_exterior_tetrahedra)
		+ sizeof(m_complete)
		+ sizeof(m_out_of_core)
		+ get_num_vertices() * sizeof(m_vertices[0])
		+ get_num_vertices() * sizeof(m_global_indices[0])
		+ (get_num_tetrahedra() + 1) * sizeof(m_tetrahedra[0])
		+ get_num_faces() * sizeof(m_faces[0])
		+ get_num_centroids() * sizeof(m_centroids[0])
		+ sizeof(m_bounding_box)
		;
}


inline const std::vector<Point_3f> &
Indexed_tetra_set::get_vertices() const
{
	return m_vertices;
}


inline std::vector<Point_3f> &
Indexed_tetra_set::get_vertices()
{
	return m_vertices;
}


inline const std::vector<unsigned> &
Indexed_tetra_set::get_vertex_global_indices() const
{
	return m_global_indices;
}


inline std::vector<unsigned> &
Indexed_tetra_set::get_vertex_global_indices()
{
	return m_global_indices;
}


inline const Tetrahedron &Indexed_tetra_set::get_tetrahedron(unsigned i) const
{
	// 0 is dummy
	GTB_REQUIRE((i > 0) && (i < m_tetrahedra.size()));
	return m_tetrahedra[i];
}


inline const std::vector<Tetrahedron> &
Indexed_tetra_set::get_tetrahedra() const
{
	return m_tetrahedra;
}


inline std::vector<Tetrahedron> &
Indexed_tetra_set::get_tetrahedra()
{
	return m_tetrahedra;
}


inline const std::vector<Indexed_tetra_set::Face> &
Indexed_tetra_set::get_faces() const
{
	return m_faces;
}


inline std::vector<Indexed_tetra_set::Face> &
Indexed_tetra_set::get_faces()
{
	return m_faces;
}


inline const std::vector<Point_3f> &
Indexed_tetra_set::get_centroids() const
{
	return m_centroids;
}


inline std::vector<Point_3f> &
Indexed_tetra_set::get_centroids()
{
	return m_centroids;
}


inline bool Indexed_tetra_set::Face::
operator==(const Indexed_tetra_set::Face &b) const
{
        const Indexed_tetra_set::Face &a = *this;
	return (a.m_vertices[0] == b.m_vertices[0] &&
		a.m_vertices[1] == b.m_vertices[1] &&
		a.m_vertices[2] == b.m_vertices[2]);
}


inline bool Indexed_tetra_set::Face::
operator<(const Indexed_tetra_set::Face &b) const
{
        const Indexed_tetra_set::Face &a = *this;
	if (a.m_vertices[0] == b.m_vertices[0]) {
		if (a.m_vertices[1] == b.m_vertices[1]) {
			return (a.m_vertices[2] < b.m_vertices[2]);
		} else {
			return (a.m_vertices[1] < b.m_vertices[1]);
		}
	} else {
		return (a.m_vertices[0] < b.m_vertices[0]);
	}
}


inline void Indexed_tetra_set::Face::set_vertices(unsigned v0,
						  unsigned v1,
						  unsigned v2)
{
	m_vertices[0] = v0;
	m_vertices[1] = v1;
	m_vertices[2] = v2;
}


inline unsigned Indexed_tetra_set::Face::get_vertex(unsigned i) const
{
	assert(i < 3);
	return m_vertices[i];
}


inline unsigned Indexed_tetra_set::Face::get_left_tetra() const
{
	return m_left_tetra;
}


inline void Indexed_tetra_set::Face::set_left_tetra(unsigned t)
{
	m_left_tetra = t;
}


inline unsigned Indexed_tetra_set::Face::get_right_tetra() const
{
	return m_right_tetra;
}


inline void Indexed_tetra_set::Face::set_right_tetra(unsigned t)
{
	m_right_tetra = t;
}


inline char Indexed_tetra_set::Face::get_index_on_left_tetra() const
{
	return m_index_on_left_tetra;
}


inline void Indexed_tetra_set::Face::set_index_on_left_tetra(char i)
{
	m_index_on_left_tetra = i;
}


inline char Indexed_tetra_set::Face::get_index_on_right_tetra() const
{
	return m_index_on_right_tetra;
}


inline void Indexed_tetra_set::Face::set_index_on_right_tetra(char i)
{
	m_index_on_right_tetra = i;
}


inline unsigned Indexed_tetra_set::Face::get_left_octree_node() const
{
	return m_left_octree_node;
}


inline void Indexed_tetra_set::Face::set_left_octree_node(unsigned on)
{
	m_left_octree_node = on;
}


inline unsigned Indexed_tetra_set::Face::get_right_octree_node() const
{
	return m_right_octree_node;
}


inline void Indexed_tetra_set::Face::set_right_octree_node(unsigned on)
{
	m_right_octree_node = on;
}


inline bool Indexed_tetra_set::Face::is_exterior() const
{
	return m_is_exterior;
}


inline void Indexed_tetra_set::Face::set_exterior(bool value)
{
	m_is_exterior = value;
}


inline const Plane_3f &Indexed_tetra_set::Face::get_plane() const
{
	return m_plane;
}


inline void Indexed_tetra_set::Face::set_plane(const Plane_3f &p)
{
	m_plane = p;
}


inline unsigned Indexed_tetra_set::get_num_vertices() const
{
	return m_vertices.size();
}


inline unsigned Indexed_tetra_set::get_num_tetrahedra() const
{
    return m_tetrahedra.size();
	//return m_tetrahedra.size() - 1; // TODO: off by one error? check all code that refers to this function
}


inline unsigned Indexed_tetra_set::get_num_primitives() const
{
	return get_num_tetrahedra();
}


inline unsigned Indexed_tetra_set::get_num_faces() const
{
	return m_faces.size();
}


inline unsigned Indexed_tetra_set::get_num_centroids() const
{
	return m_centroids.size();
}


inline unsigned Indexed_tetra_set::get_num_exterior_faces() const
{
	return m_num_exterior_faces;
}


inline unsigned Indexed_tetra_set::get_num_exterior_tetrahedra() const
{
	return m_num_exterior_tetrahedra;
}


inline bool Indexed_tetra_set::is_complete() const
{
	return m_complete;
}

inline bool Indexed_tetra_set::is_out_of_core() const
{
	return m_out_of_core;
}


inline unsigned Indexed_tetra_set::get_global_vertex(unsigned local_vertex)
{
	GTB_REQUIRE(local_vertex < m_global_indices.size());
	return m_global_indices[local_vertex];
}


GTB_END_NAMESPACE

#endif // GTB_INDEXED_TETRA_SET_INCLUDED
