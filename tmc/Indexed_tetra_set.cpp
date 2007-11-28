// Based on code by Peter Shirley, Peter L. Williams, Nelson Max, and
// Claudio Silva

#ifndef GTB_INDEXED_TETRA_SET_INCLUDED
#include "Indexed_tetra_set.hpp"
#endif

#ifndef GTB_CXX_SET_INCLUDED
#include <set>
#define GTB_CXX_SET_INCLUDED
#endif

//#ifndef GTB_IO_INCLUDED
//#include "io.hpp"
//#endif

// #ifndef GTB_TETRA_SORTER_INCLUDED
// #include "Tetra_sorter.hpp"
// #endif

#ifndef GTB_EXTERIOR_FACE_INCLUDED
#include "Exterior_face.hpp"
#endif

// #ifndef GTB_RENDERING_CONTEXT_INCLUDED
// #include "Rendering_context.hpp"
// #endif

#ifndef GTB_VISITOR_INCLUDED
#include "Visitor.hpp"
#endif

// #ifndef GTB_SHAPE_FILE_INCLUDED
// #include "Shape_file.hpp"
// #endif

#include "math.hpp"

#define DBL_EPS 1.e-10

GTB_BEGIN_NAMESPACE

const char *Indexed_tetra_set::m_class_name = "Indexed_tetra_set";
// static int s_nifty_counter = 0;


Indexed_tetra_set_init::Indexed_tetra_set_init()
{
// 	if (0 == s_nifty_counter++) {
// 		GTB_SHAPE_FILE_REGISTER_READER(Indexed_tetra_set);
// 	}
}


Indexed_tetra_set_init::~Indexed_tetra_set_init()
{
// 	if (0 == --s_nifty_counter) {
// 		// nothing
// 	}
}


#define FREAD(x, fp) fread(&x, sizeof(x), 1, fp)
#define FWRITE(x, fp) fwrite(&x, sizeof(x), 1, fp)


struct Tetra_set_file_header {
	unsigned id;
	bool complete;
	bool out_of_core;
	unsigned num_vertices;
	unsigned num_global_indices;
	unsigned num_faces;
	unsigned num_tetrahedra;
	unsigned num_centroids;
	Bounding_box_3d bounding_box;

	Tetra_set_file_header(const Indexed_tetra_set &its)
		: id(its.get_id()), 
		  complete(its.is_complete()),
		  out_of_core(its.is_out_of_core()),
		  num_vertices(its.get_num_vertices()),
		  num_global_indices(its.get_num_vertices()),
		  num_faces(its.get_num_faces()),
		  num_tetrahedra(its.get_num_tetrahedra() + 1),
		  num_centroids(its.get_num_centroids()),
		  bounding_box(its.get_bounding_box()) {
	}

	Tetra_set_file_header(FILE *fp) {
		if ((FREAD(id, fp) +
		     FREAD(complete, fp) +
		     FREAD(out_of_core, fp) +
		     FREAD(num_vertices, fp) +
		     FREAD(num_global_indices, fp) +
		     FREAD(num_faces, fp) +
		     FREAD(num_tetrahedra, fp) +
		     FREAD(num_centroids, fp) +
		     FREAD(bounding_box, fp))
		    != 9) {
			GTB_ERROR("failed to read header");
		}
	}

	void write(FILE *fp) const {
		if ((FWRITE(id, fp) +
		     FWRITE(complete, fp) +
		     FWRITE(out_of_core, fp) +
		     FWRITE(num_vertices, fp) +
		     FWRITE(num_global_indices, fp) +
		     FWRITE(num_faces, fp) +
		     FWRITE(num_tetrahedra, fp) +
		     FWRITE(num_centroids, fp) +
		     FWRITE(bounding_box, fp))
		    != 9) {
			GTB_ERROR("failed to write header");
		}
	}
};


Indexed_tetra_set::Indexed_tetra_set()
	: m_id(0), 
	  m_complete(false),
	  m_out_of_core(false)
{
}


Indexed_tetra_set::Indexed_tetra_set(
	const std::vector<Point_3f> &vertices, 
	const std::vector<Tetrahedron> &tetrahedra)
	: m_vertices(vertices),
	  m_tetrahedra(tetrahedra),
	  m_id(0), 
	  m_complete(true),
	  m_out_of_core(false)
{
	// create the global indices
	m_global_indices.resize(get_num_vertices());
	for (unsigned i = 0; i < get_num_vertices(); i++) {
		m_global_indices[i] = i;
	}
 
	// complete the creation of the indexed tetra set
	m_bounding_box = Bounding_box_3d(m_vertices);
#if 0
	build_tetrahedra();
	build_centroids();
	build_faces();
#endif
	set_id(0);
}


Indexed_tetra_set::Indexed_tetra_set(
	const std::vector<Point_3f> &vertices, 
	const std::vector<unsigned> &global_indices,
	const std::vector<Tetrahedron> &tetrahedra)
	: m_vertices(vertices),
	  m_tetrahedra(tetrahedra),
	  m_global_indices(global_indices),
	  m_id(0),
	  m_complete(false),
	  m_out_of_core(true)
{
	m_bounding_box = Bounding_box_3d(m_vertices);
}


Indexed_tetra_set::Indexed_tetra_set(
	const std::vector<Point_3f> &vertices, 
	const std::vector<Tetrahedron> &tetrahedra,
	bool replicate)
	: m_vertices(vertices),
	  m_tetrahedra(tetrahedra),
	  m_id(0), 
	  m_complete(false),
	  m_out_of_core(false)
{
	(void) replicate;
	// create the global indices
	m_global_indices.resize(get_num_vertices());
	for (unsigned i = 0; i < get_num_vertices(); i++) {
		m_global_indices[i] = i;
	}
 
	// complete the creation of the indexed tetra set
	m_bounding_box = Bounding_box_3d(m_vertices);
}


struct Vertex_tuple {
	Vertex_tuple (unsigned gi, unsigned li) {
		global_index = gi;
		local_index = li;
	}
	
	friend bool operator==(const Vertex_tuple &a, const Vertex_tuple &b) {
		return (a.global_index == b.global_index);
	}
	friend bool operator<(const Vertex_tuple &a, const Vertex_tuple &b) {
		return (a.global_index < b.global_index);
	}
	
	unsigned global_index;
	unsigned local_index;
};


Indexed_tetra_set::Indexed_tetra_set(
	const Indexed_tetra_set &src, 
	const std::vector<Tetrahedron> &src_tetra)
	: m_id(0),
	  m_complete(false),
	  m_out_of_core(true)
{
	GTB_CHECK(are_tetrahedra_vertices_valid());
 	std::set<Vertex_tuple> vertex_tuple_set;
	std::set<Vertex_tuple>::const_iterator vti;
	const std::vector<Point_3f> &src_vertices = src.get_vertices();
	const std::vector<unsigned> &src_vertex_global_indices = 
		src.get_vertex_global_indices();

	m_vertices.reserve(src_tetra.size() * 4);
	m_tetrahedra.reserve(src_tetra.size() + 1); // +1 because of dummy
	m_global_indices.reserve(src_tetra.size() * 4);	

	// the dummy
	m_tetrahedra.push_back(Tetrahedron(0, 0, 0, 0));

	for (unsigned i = 0; i < src_tetra.size(); i++) {
 		const Tetrahedron &T1 = src_tetra[i];
		unsigned vi[4];
			
		for (char j = 0; j < 4; j++) {
			unsigned vertex = T1.get_vertex(j);
			if (vertex >= src_vertex_global_indices.size()) {
				GTB_ERROR3("vertex=%d  svgi.size=%d\n",
					   vertex,
					   src_vertex_global_indices.size());
			}
			unsigned vgi = src_vertex_global_indices[vertex];
			Vertex_tuple tuple(vgi,0);
			vti = vertex_tuple_set.find(tuple);

			if (vti == vertex_tuple_set.end()) {
				Point_3f p = src_vertices[vertex];
				m_vertices.push_back(p);
				m_global_indices.push_back(vgi);
				
				vi[j] = m_vertices.size() - 1;
				tuple.local_index = vi[j];
				vertex_tuple_set.insert(tuple);
			} else {
				vi[j] = (*vti).local_index;
			}
		}
			
		Tetrahedron T2(vi[0], vi[1], vi[2], vi[3]);
		m_tetrahedra.push_back(T2);	
	}

 	m_bounding_box = Bounding_box_3d(m_vertices);
	GTB_CHECK(are_tetrahedra_vertices_valid());
}


void Indexed_tetra_set::add(const Indexed_tetra_set &src)
{
	GTB_CHECK(are_tetrahedra_vertices_valid());
	GTB_CHECK(src.are_tetrahedra_vertices_valid());
 	std::set<Vertex_tuple> vertex_tuple_set;
	std::set<Vertex_tuple>::const_iterator vti;
	const std::vector<Point_3f> &src_vertices = src.get_vertices();
	const std::vector<unsigned> &src_vertex_global_indices = 
		src.get_vertex_global_indices();
	const std::vector<Tetrahedron> &src_tetra = src.get_tetrahedra();

	m_complete = false;
	m_out_of_core = true;

	m_vertices.reserve(get_num_vertices() + src.get_num_vertices());
	m_tetrahedra.reserve(get_num_tetrahedra() + src.get_num_tetrahedra());
	m_global_indices.reserve(get_num_vertices() + 
					src.get_num_vertices());	

	for (unsigned i = 0; i < get_num_vertices(); i++) {
		unsigned vgi = m_global_indices[i];
		Vertex_tuple tuple(vgi,i);
		vertex_tuple_set.insert(tuple);
	}

	// skip dummy
	for (unsigned i = 1; i < src.get_num_tetrahedra(); i++) {
 		const Tetrahedron &T1 = src_tetra[i];
		GTB_CHECK(T1.are_vertices_valid());
		unsigned vi[4];
			
		for (char j = 0; j < 4; j++) {
			unsigned vertex = T1.get_vertex(j);
			unsigned vgi = src_vertex_global_indices[vertex];
			Vertex_tuple tuple(vgi,0);
			vti = vertex_tuple_set.find(tuple);

			if (vti == vertex_tuple_set.end()) {
				Point_3f p = src_vertices[vertex];
				m_vertices.push_back(p);
				m_global_indices.push_back(vgi);
				
				vi[j] = m_vertices.size() - 1;
				tuple.local_index = vi[j];
				vertex_tuple_set.insert(tuple);
 			} else {
				vi[j] = (*vti).local_index;
			}
		}
			
		Tetrahedron T2(vi[0], vi[1], vi[2], vi[3]);
		m_tetrahedra.push_back(T2);	
	}

 	m_bounding_box = Bounding_box_3d(m_vertices);
	GTB_CHECK(are_tetrahedra_vertices_valid());
}


void Indexed_tetra_set::build(unsigned octree_node_id)
{
#if 0
	if (is_complete()) {
		return;
	}
#endif
	m_complete = true;

	build_tetrahedra();
	build_centroids();
	build_faces();
	set_id(octree_node_id);
}


void Indexed_tetra_set::shift(float z_shift)
{
	printf("\n replicate vertices and global indices\n");

	unsigned i;
	for (i = 0; i < get_num_vertices(); i++) {
		m_vertices[i].set_z(m_vertices[i].get_z() + z_shift);
		m_global_indices[i] = m_global_indices[i] + 
			get_num_vertices();
	}

 	m_bounding_box = Bounding_box_3d(m_vertices);
}


void Indexed_tetra_set::write(FILE * fp) const
{
	GTB_REQUIRE(fp != NULL);

	// write header
	Tetra_set_file_header header(*this);
	header.write(fp);

        // write vertices
	if (header.num_vertices > 0) {
		if (fwrite(&m_vertices[0],
			   header.num_vertices * sizeof(m_vertices[0]),
			   1, fp) != 1) {
			GTB_ERROR("failed to write vertices");
		}
	}

	// write faces
	if (header.num_faces > 0) {
		if (fwrite(&m_faces[0],
			   header.num_faces * sizeof(m_faces[0]),
			   1, fp) != 1) {
			GTB_ERROR("failed to write faces");
		}
	}

	// write tetrahedra
	if (header.num_tetrahedra > 0) {
		if (fwrite(&m_tetrahedra[0],
			   header.num_tetrahedra * sizeof(m_tetrahedra[0]),
			   1, fp) != 1) {
			GTB_ERROR("failed to write tetrahedra");
		}
	}

	// write centroids
	if (header.num_centroids > 0) {
		if (fwrite(&m_centroids[0],
			   header.num_centroids * sizeof(m_centroids[0]),
			   1, fp) != 1) {
			GTB_ERROR("failed to write centroids");
		}
	}

        // write vertex global indices
	if (header.num_global_indices > 0) {
		if (fwrite(&m_global_indices[0],
			   header.num_global_indices * 
			   sizeof(m_global_indices[0]),
			   1, fp) != 1) {
			GTB_ERROR("failed to write vertex global indices");
		}
	}

}


Indexed_tetra_set::Indexed_tetra_set(FILE *fp)
{
	GTB_REQUIRE(fp != NULL);

	// read header
	Tetra_set_file_header header(fp);

	m_id = header.id;
	m_complete = header.complete;
	m_out_of_core = header.out_of_core;
	m_bounding_box = header.bounding_box;

        // read vertices
	if (header.num_vertices > 0) {
		m_vertices.resize(header.num_vertices);
		if (fread(&m_vertices[0],
			  header.num_vertices * sizeof(m_vertices[0]),
			  1, fp) != 1) {
			GTB_ERROR("failed to read vertices");
		}
	}

	// read faces
	if (header.num_faces > 0) {
		m_faces.resize(header.num_faces);
		if (fread(&m_faces[0],
			  header.num_faces * sizeof(m_faces[0]),
			  1, fp) != 1) {
			GTB_ERROR("failed to read faces");
		}
	}

	// read tetrahedra
	if (header.num_tetrahedra > 0) {
		m_tetrahedra.resize(header.num_tetrahedra);
		if (fread(&m_tetrahedra[0],
			  header.num_tetrahedra * sizeof(m_tetrahedra[0]),
			  1, fp) != 1) {
			GTB_ERROR("failed to read tetrahedra");
		}
	}

	// read centroids
	if (header.num_centroids > 0) {
		m_centroids.resize(header.num_centroids);
		if (fread(&m_centroids[0],
			  header.num_centroids * sizeof(m_centroids[0]),
			  1, fp) != 1) {
			GTB_ERROR("failed to read centroids");
		}
	}

        // read global indices
	if (header.num_global_indices > 0) {
		m_global_indices.resize(header.num_global_indices);
		if (fread(&m_global_indices[0],
			  header.num_global_indices * 
			  sizeof(m_global_indices[0]),
			  1, fp) != 1) {
			GTB_ERROR("failed to read vertex global indices");
		}
	}

	//print_status();
}


Indexed_tetra_set::~Indexed_tetra_set()
{
}


void Indexed_tetra_set::print_status()
{
 	printf (" complete state : %d\n", is_complete());
 	printf (" num vertices   : %d\n", get_num_vertices());
	printf (" num tetra      : %d\n", get_num_tetrahedra());
	printf (" num faces      : %d\n", get_num_faces());
	printf (" num centroids  : %d\n", get_num_centroids());
	printf (" num ext faces  : %d\n", get_num_exterior_faces());
	printf (" num ext tetra  : %d\n", get_num_exterior_tetrahedra());

	for(unsigned i = 0; i < get_num_vertices(); i++) {
		Point_3f &p = m_vertices[i];
		printf("  v[%d] = %f, %f, %f\n",i, p[0], p[1], p[2]);
	}

	printf("\n");
	for(unsigned i = 0; i <= get_num_tetrahedra(); i++) {
		Tetrahedron &t = m_tetrahedra[i];
		printf("  t[%d] = %d   f[0] = %2d  -> t = %2d   ext = %d\n"
		       "         %d   f[1] = %2d  ->  t = %2d   ext = %d\n"
		       "         %d   f[2] = %2d  ->  t = %2d   ext = %d\n"
		       "         %d   f[3] = %2d  ->  t = %2d   ext = %d\n\n", 
		       i,
		       t.get_vertex(0),t.get_face(0),t.get_neighbor(0),
		       t.is_face_exterior(0), 
		       t.get_vertex(1),t.get_face(1),t.get_neighbor(1),
		       t.is_face_exterior(1), 
		       t.get_vertex(2),t.get_face(2),t.get_neighbor(2),
		       t.is_face_exterior(2), 
		       t.get_vertex(3),t.get_face(3),t.get_neighbor(3),
		       t.is_face_exterior(3)); 
	}
	
	printf("\n");
	for(unsigned i=0; i < get_num_faces(); i++) {
		Indexed_tetra_set::Face &f = m_faces[i];
		printf("  f[%2d] = %d, %d, %d     ext = %d      t[%d][%d]  <=>  t[%d][%d]\n", 
		       i, 
		       f.get_vertex(0), f.get_vertex(1), f.get_vertex(2),
		       f.is_exterior(),
		       f.get_left_tetra(), f.get_index_on_left_tetra(),
		       f.get_right_tetra(), f.get_index_on_right_tetra());
	}

	printf("\n");
	for(unsigned i=0; i < get_num_centroids(); i++) {
		printf("  c[%d] = %f %f %f\n",
		       i, 
		       m_centroids[i][0], 
		       m_centroids[i][1], 
		       m_centroids[i][2]);
	}
}


void Indexed_tetra_set::set_id(unsigned id)
{
	m_id = id;

// 	printf("octree node id = %d\n", id);

	// set the tetrahedron neighbors octree nodes

	// FIXME: why does get_num_tetrahedra() return
	// m_tetrahedra.size() - 1, instead of m_tetrahedra.size().
	for (unsigned i = 0; i < get_num_tetrahedra(); i++) {
		GTB_CHECK(i < m_tetrahedra.size());
		m_tetrahedra[i].set_octree_node(id);
		for (int f = 0; f < 4; f++) {
			m_tetrahedra[i].set_neighbor_octree_node(f, id);
		}
	}

	// set the faces left and right octree nodes

	// FIXME: Here was the bug:
	// for (unsigned i = 0; i <= get_num_faces(); i++) {
	for (unsigned i = 0; i < get_num_faces(); i++) {
		GTB_CHECK(i < m_faces.size());
		m_faces[i].set_left_octree_node(id);
		m_faces[i].set_right_octree_node(id);
	}
}


Indexed_tetra_set::Face::Face()
	: m_left_tetra(0),
	  m_right_tetra(0),
	  m_index_on_left_tetra(-1),
	  m_index_on_right_tetra(-1),
	  m_left_octree_node(0),
	  m_right_octree_node(0),
	  m_is_exterior(false)
{
	m_vertices[0] = 0;
	m_vertices[1] = 0;
	m_vertices[2] = 0;
}


Indexed_tetra_set::Face::Face(unsigned i, unsigned j, unsigned k)
	: m_left_tetra(0),
	  m_right_tetra(0),
	  m_index_on_left_tetra(-1),
	  m_index_on_right_tetra(-1),
	  m_left_octree_node(0),
	  m_right_octree_node(0),
	  m_is_exterior(false)
{
	GTB_CHECK((i != j) && (i != k) && (j != k));
	m_vertices[0] = i;
	m_vertices[1] = j;
	m_vertices[2] = k;
	std::sort(&m_vertices[0], &m_vertices[3]);
}


void Indexed_tetra_set::Face::set_tetra(int t)
{
	GTB_CHECK(t > 0);
	if (m_left_tetra == 0) {
		m_left_tetra = t;
	} else if (m_right_tetra == 0) {
		m_right_tetra = t;
	} else {
		fprintf(stderr, "m_left_tetra=%d ", m_left_tetra);
		fprintf(stderr, "m_right_tetra=%d ", m_right_tetra);
		fprintf(stderr, "m_vertices=(%d %d %d)\n",
			m_vertices[0], m_vertices[1], m_vertices[2]);
		GTB_ERROR("more than 2 tetra share face");
	}
}


bool Indexed_tetra_set::are_tetrahedra_vertices_valid() const
{
	bool result = true;

	// skip 0, because it's the dummy tetrahedron
	for (unsigned i = 1; i < m_tetrahedra.size(); ++i) {
		const Tetrahedron &t = m_tetrahedra[i];
		if (!t.are_vertices_valid()) {
			fprintf(stderr, "tetra set: %d  ", m_id);
			fprintf(stderr, "bad tetrahedron: %d  ", i);
			fprintf(stderr, "vertices: %d %d %d %d\n",
				t.get_vertex(0),
				t.get_vertex(1),
				t.get_vertex(2),
				t.get_vertex(3));
			result = false;
			break;
		}
	}
	return result;
}


void Indexed_tetra_set::compute_connectivity()
{
	GTB_CHECK(are_tetrahedra_vertices_valid());
	std::set<Face> face_set;

	// create faces
	unsigned num_tetrahedra = get_num_tetrahedra();
	for (unsigned i = 1; i <= num_tetrahedra; i++) {
		for (int f = 0; f < 4; f++) {
			Face ft;
			switch (f) {
			case 0:
				ft = Face(m_tetrahedra[i].get_vertex(0),
					  m_tetrahedra[i].get_vertex(1),
					  m_tetrahedra[i].get_vertex(2));
				break;
			case 1:
				ft = Face(m_tetrahedra[i].get_vertex(1),
					  m_tetrahedra[i].get_vertex(2),
					  m_tetrahedra[i].get_vertex(3));
				break;
			case 2:
				ft = Face(m_tetrahedra[i].get_vertex(0),
					  m_tetrahedra[i].get_vertex(1),
					  m_tetrahedra[i].get_vertex(3));
				break;
			case 3:
				ft = Face(m_tetrahedra[i].get_vertex(0),
					  m_tetrahedra[i].get_vertex(2),
					  m_tetrahedra[i].get_vertex(3));
				break;
			}

			std::set<Face>::iterator fi = face_set.find(ft);
			if (fi == face_set.end()) {
				ft.set_tetra(i);
				face_set.insert(ft);
			} else {
				Face aux = *fi;
				aux.set_tetra(i);
				face_set.erase(fi);
				face_set.insert(aux);
			}
		}
	}

	// compute neighbors
	for (unsigned i = 1; i <= num_tetrahedra; i++) {
		for (int f = 0; f < 4; f++) {
			Face ft;
			switch (f) {
			case 3:
				ft = Face(m_tetrahedra[i].get_vertex(0),
					  m_tetrahedra[i].get_vertex(1),
					  m_tetrahedra[i].get_vertex(2));
				break;
			case 0:
				ft = Face(m_tetrahedra[i].get_vertex(1),
					  m_tetrahedra[i].get_vertex(2),
					  m_tetrahedra[i].get_vertex(3));
				break;
			case 2:
				ft = Face(m_tetrahedra[i].get_vertex(0),
					  m_tetrahedra[i].get_vertex(1),
					  m_tetrahedra[i].get_vertex(3));
				break;
			case 1:
				ft = Face(m_tetrahedra[i].get_vertex(0),
					  m_tetrahedra[i].get_vertex(2),
					  m_tetrahedra[i].get_vertex(3));
				break;
			}

			std::set <Face>::const_iterator fi = face_set.find(ft);
			if (fi == face_set.end()) {
				GTB_ERROR("could not find face");
			}

			if ((*fi).get_left_tetra() != i) {
				m_tetrahedra[i].set_neighbor(
					f, (*fi).get_left_tetra());
				GTB_CHECK((*fi).get_right_tetra() == i);
			} else if ((*fi).get_right_tetra() != i) {
				m_tetrahedra[i].set_neighbor(
					f, (*fi).get_right_tetra());
				GTB_CHECK((*fi).get_left_tetra() == i);
			}
		}
	}
// 	printf("face_set.size() = %d\n", face_set.size());
}


void Indexed_tetra_set::build_tetrahedra()
{
	compute_connectivity();
 
	m_num_exterior_faces = 0;
	m_num_exterior_tetrahedra = 0;
	unsigned face_count = 0;

	for (unsigned t = 1; t <= get_num_tetrahedra(); t++) {
		bool first_exterior_face = true;
		for (int f = 0; f < 4; f++) {
			if (m_tetrahedra[t].is_face_exterior(f)) {
				/* use the neighbor field to point to
				 * the centroid (oh boy...) */
				m_num_exterior_faces++;
				m_tetrahedra[t].set_face(f, face_count++);
				if (first_exterior_face) {
					first_exterior_face = false;
					m_num_exterior_tetrahedra++;
					m_tetrahedra[t].set_neighbor(
						f, -m_num_exterior_tetrahedra);
				} else {
					m_tetrahedra[t].set_neighbor(
						f, -m_num_exterior_tetrahedra);
				}
			} else {
				GTB_CHECK(m_tetrahedra[t].get_neighbor(f) > 0);
				unsigned st = m_tetrahedra[t].get_neighbor(f);
				if (st > t) {
					/* the tetrahedron with lower
					 * index is responsible for
					 * setting the face number on
					 * both tetrahedra of the face */

					int fn = face_count++;
					m_tetrahedra[t].set_face(f, fn);
					unsigned i;
					for (i = 0; i < 4; i++) {
						if (m_tetrahedra[st].
						    get_neighbor(i)
						    == (int) t) {
							break;
						}
					}
					m_tetrahedra[st].set_face(i, fn);
				}
			}
		}
	}

	m_faces.resize(face_count);

// 	fprintf(stderr, "m_num_exterior_faces=%d\n", m_num_exterior_faces);
// 	fprintf(stderr, "m_num_exterior_tetrahedra=%d\n", m_num_exterior_tetrahedra);
// 	fprintf(stderr, "face_count=%d\n", face_count);
}


void Indexed_tetra_set::build_centroids()
{
	m_centroids.resize(m_num_exterior_tetrahedra + 1);
	for (unsigned tet = 1; tet <= get_num_tetrahedra(); tet++) {
		for (int f = 0; f <= 3; f++) {
			if (m_tetrahedra[tet].is_face_exterior(f)) {
				int i = -m_tetrahedra[tet].get_neighbor(f);
				m_centroids[i] = Point_3f::centroid(
					m_vertices[m_tetrahedra[tet].
						   get_vertex(0)],
					m_vertices[m_tetrahedra[tet].
						   get_vertex(1)],
					m_vertices[m_tetrahedra[tet].
						   get_vertex(2)],
					m_vertices[m_tetrahedra[tet].
						   get_vertex(3)]);
			}
		}
	}
}


void Indexed_tetra_set::Face::orient_normal(const Point_3f &p)
{
	if (m_plane.signed_distance(p) > 0.0) {
		m_plane.flip();
		unsigned aux = m_vertices[1];
		m_vertices[1] = m_vertices[2];
		m_vertices[2] = aux;
	}
}


// Why do we need this separate function?  Why don't we keep the map
// in compute_connectivity() instead?  We are losing the vertex
// indices inside the faces.  Do we ever need them?
void Indexed_tetra_set::build_faces()
{
	for (unsigned ti = 1; ti <= get_num_tetrahedra(); ti++) {
		Tetrahedron &t = m_tetrahedra[ti];
		for (unsigned fi = 0; fi <= 3; fi++) {
			int st = t.get_neighbor(fi);
			if (!t.is_face_exterior(fi) && ((int) ti >= st)) {
				continue;
			}
			Face &f = m_faces[t.get_face(fi)];
			f.set_left_tetra(ti);
			f.set_index_on_left_tetra(fi);
			if (t.is_face_exterior(fi)) {
				f.set_exterior(true);
				f.set_right_tetra(st);
			} else {
				f.set_exterior(false);
				f.set_right_tetra(st);
				Tetrahedron &tst = m_tetrahedra[st];
				for (int i = 0; i < 4; i++) {
					if (tst.get_neighbor(i) == (int) ti) {
						f.set_index_on_right_tetra(i);
						break;
					}
				}
			}
			unsigned v[4];
			t.get_face_indices(fi, v);
			f.set_vertices(v[0], v[1], v[2]);
			f.set_plane(Plane_3f(m_vertices[v[0]],
					     m_vertices[v[1]],
					     m_vertices[v[2]]));
			f.orient_normal(m_vertices[v[3]]);
		}
	}
}


void Indexed_tetra_set::export_exterior_faces(FILE *fp)
{
	for(unsigned i = 0; i < get_num_faces(); i++) {
		Face &F = m_faces[i];
		if (!F.is_exterior()) {
			continue;
		}
		unsigned v[3];
		if (is_out_of_core()) {
			v[0] = m_global_indices[F.get_vertex(0)];
			v[1] = m_global_indices[F.get_vertex(1)];
			v[2] = m_global_indices[F.get_vertex(2)];
		} else {
			v[0] = F.get_vertex(0);
			v[1] = F.get_vertex(1);
			v[2] = F.get_vertex(2);
		}
		std::sort(&v[0], &v[3]);

		unsigned left_tetra = F.get_left_tetra();
		unsigned left_octree_node = F.get_left_octree_node();
		char index_on_left_tetra = F.get_index_on_left_tetra();
		const Tetrahedron &T = m_tetrahedra[left_tetra]; 
		unsigned face = T.get_face(index_on_left_tetra);

		Exterior_face ef(v, 
				 left_octree_node,
				 left_tetra,
				 index_on_left_tetra,
				 face);
		ef.write(fp);
	}
}


void Indexed_tetra_set::set_as_interior_face(
	unsigned face,
	unsigned index_on_left_tetra,
	unsigned right_octree_node,
	unsigned right_tetra,
	unsigned index_on_right_tetra)
{
	GTB_CHECK(face < m_faces.size());
	GTB_CHECK(index_on_left_tetra < 4);
	GTB_CHECK(index_on_right_tetra < 4);

	// update face
	Face &F = m_faces[face];
	F.set_right_octree_node(right_octree_node);
	F.set_right_tetra(right_tetra);
	F.set_index_on_right_tetra(index_on_right_tetra);
	F.set_exterior(false);
 
	// update tetrahedron
	unsigned left_tetra = F.get_left_tetra();
	Tetrahedron &LT = m_tetrahedra[left_tetra]; 
	LT.set_neighbor_octree_node(index_on_left_tetra, right_octree_node);
	LT.set_neighbor(index_on_left_tetra, right_tetra);
}


Shape *Indexed_tetra_set::read(FILE *fp)
{
	return new Indexed_tetra_set(fp);
}


// void Indexed_tetra_set::render(Rendering_context &rc)
// {
// 	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT);
// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 	glDisable(GL_TEXTURE_2D);
// 	glDisable(GL_CULL_FACE);
// 	glDisable(GL_DEPTH_TEST);
// 	glDepthMask(GL_FALSE);
// 	glDisable(GL_LIGHTING);
// 	glShadeModel(GL_SMOOTH);

// 	GTB_CHECK(is_complete());

// 	if (rc.is_enabled(rc.DRAW_BOUNDING_BOX)) {
// 		rc.get_bounding_box_color().gl_load();
// 		get_bounding_box().outline();
// 	}

// 	if (rc.is_enabled(rc.DRAW_CONTENTS)) {
// 		Vector_3d t = rc.get_view().get_camera().get_towards();
// 		Vector_3f pd(t[0], t[1], t[2]);

// 		Tetra_sorter *sorter = rc.get_tetra_sorter();
// 		Tetra_splatter *splatter = rc.get_tetra_splatter();
// 		Scalar_colors *C = rc.get_scalar_colors();
// 		Scalar_densities *D = rc.get_scalar_densities();
// 		Color_map *color_map = rc.get_color_map();

// 		GTB_CHECK(sorter != NULL);
// 		GTB_CHECK(splatter != NULL);
// 		GTB_CHECK(C != NULL);
// 		GTB_CHECK(D != NULL);
// 		GTB_CHECK(color_map != NULL);

// 		sorter->compute_visibility_graph(*this, pd);
// 		sorter->sort_visibility_graph(*this,
// 					      *C, *D, *color_map,
// 					      *splatter, pd);
// 	}

// 	glPopAttrib();
// }


// void Indexed_tetra_set::render_geometry(Rendering_context &rc)
// { 
// 	(void) rc;
// 	for (unsigned ti = 1; ti < m_tetrahedra.size(); ti++) {
// 		const Tetrahedron &t = m_tetrahedra[ti];
// 		for (unsigned fi = 0; fi < 4; fi++) {
// 			const Indexed_tetra_set::Face &f =
// 				m_faces[t.get_face(fi)];
// 			Color_3f c;
// 			c = get_color_3f(GTB_WHITE);
// 			glBegin(GL_LINE_LOOP);
// 			c.gl_load();
// 			m_vertices[f.get_vertex(0)].gl_vertex();
// 			m_vertices[f.get_vertex(1)].gl_vertex();
// 			m_vertices[f.get_vertex(2)].gl_vertex();
// 			glEnd();  
// 		}
// 	}
// }


bool Indexed_tetra_set::approx_inside_tetra(const Tetrahedron &t, const Point_3f &p)
{
	for (unsigned fi = 0; fi < 4; fi++) {

		unsigned v[4];
		t.get_face_indices(fi, v);

#if 1
		Face f;
		f.set_vertices(v[0], v[1], v[2]);
		f.set_plane(Plane_3f(m_vertices[v[0]],
				     m_vertices[v[1]],
				     m_vertices[v[2]]));
#else
		Face &f = m_faces[t.get_face(fi)];
#endif
		// orient the normal to the outside
		f.orient_normal(m_vertices[v[3]]);

		float dist = f.get_plane().signed_distance(p);
 		if (eps_is_positive(dist, 1e-5)) {
			// the point is on the outside of the face
			return false;
		}
	}
	return true;
}


bool Indexed_tetra_set::interpol_tetra(Scalar_time_series *ts, const Tetrahedron &t, const Point_3f &V, float *result)
{ 
	bool inside = true;

    // compute barycentric coordinates
    // M = (A-D|B-D|C-D)
    Point_3f &D = m_vertices[t.get_vertex(3)];
    Vector_3f C0 = m_vertices[t.get_vertex(0)] - D;
    Vector_3f C1 = m_vertices[t.get_vertex(1)] - D;
    Vector_3f C2 = m_vertices[t.get_vertex(2)] - D;
    float m[][4] = { { C0[0], C1[0], C2[0], 0 },
                     { C0[1], C1[1], C2[1], 0 },
                     { C0[2], C1[2], C2[2], 0 },
                     { 0, 0, 0, 1 }
    };
	Matrix_4d M(m);

#if 1
	Matrix_4d a(M);
	Matrix_4d b(MATRIX_4D_IDENTITY);
	int p[4];
	if (!a.LU_decomposition(p)) {
        //fprintf(stderr, "Degenerated tet\n");
		return false;
	}
	a.LU_back_substitution(p, b, M);
#else
	M = M.inverse();
#endif

	// coords = M^-1 * (V-D)
	Vector_3f coords3 = M * (V - D);

	float coords4[4];
	memcpy(&coords4[0], &coords3[0], 3*sizeof(float));
	coords4[3] = 1 - coords3[0] - coords3[1] - coords3[2];

	if (!eps_is_positive_or_zero(coords4[0], DBL_EPS) ||
	    !eps_is_positive_or_zero(coords4[1], DBL_EPS) ||
	    !eps_is_positive_or_zero(coords4[2], DBL_EPS) ||
	    !eps_is_positive_or_zero(coords4[3], DBL_EPS)) {
//		fprintf(stderr, "wrong barycenter: %e %e %e %e\n",
//			coords4[0],coords4[1],coords4[2],coords4[3]);
		inside = false;
	}

	float total_val = 0;
	for (unsigned i=0; i<4; i++) {
		unsigned gi = get_global_vertex(t.get_vertex(i));
		float Si = ts->get_value(gi);
		total_val += coords4[i] * Si;
	}

	*result = total_val;
	return inside;
}

bool Indexed_tetra_set::interpol_tetra(Vector_time_series *ts, const Tetrahedron &t, const Point_3f &V, Vector_3f *result)
{ 
	bool inside = true;

    // compute barycentric coordinates
    // M = (A-D|B-D|C-D)
    Point_3f &D = m_vertices[t.get_vertex(3)];
    Vector_3f C0 = m_vertices[t.get_vertex(0)] - D;
    Vector_3f C1 = m_vertices[t.get_vertex(1)] - D;
    Vector_3f C2 = m_vertices[t.get_vertex(2)] - D;
    float m[][4] = { { C0[0], C1[0], C2[0], 0 },
                     { C0[1], C1[1], C2[1], 0 },
                     { C0[2], C1[2], C2[2], 0 },
                     { 0, 0, 0, 1 }
    };
	Matrix_4d M(m);

#if 1
	Matrix_4d a(M);
	Matrix_4d b(MATRIX_4D_IDENTITY);
	int p[4];
	if (!a.LU_decomposition(p)) {
        //fprintf(stderr, "Degenerated tet\n");
		return false;
	}
	a.LU_back_substitution(p, b, M);
#else
	M = M.inverse();
#endif

	// coords = M^-1 * (V-D)
	Vector_3f coords3 = M * (V - D);

	float coords4[4];
	memcpy(&coords4[0], &coords3[0], 3*sizeof(float));
	coords4[3] = 1 - coords3[0] - coords3[1] - coords3[2];

	if (!eps_is_positive_or_zero(coords4[0], DBL_EPS) ||
	    !eps_is_positive_or_zero(coords4[1], DBL_EPS) ||
	    !eps_is_positive_or_zero(coords4[2], DBL_EPS) ||
	    !eps_is_positive_or_zero(coords4[3], DBL_EPS)) {
//		fprintf(stderr, "wrong barycenter: %e %e %e %e\n",
//			coords4[0],coords4[1],coords4[2],coords4[3]);
		inside = false;
	}

	Vector_3f total_val(0,0,0);
	for (unsigned i=0; i<4; i++) {
		unsigned gi = get_global_vertex(t.get_vertex(i));
		Vector_3f Si = ts->get_value(gi);
		total_val += coords4[i] * Si;
	}

	*result = total_val;
	return inside;
}

bool Indexed_tetra_set::eval(Scalar_time_series *ts, const Point_3f &p, float *result)
{
	for (unsigned ti = 1; ti < m_tetrahedra.size(); ti++) {
		const Tetrahedron &t = m_tetrahedra[ti];
		if (!approx_inside_tetra(t, p)) continue;
		float value;
		if (interpol_tetra(ts, t, p, &value)) {
			*result = value;
			return true;
		}
	}
	return false;
}

void Indexed_tetra_set::accept(Visitor &visitor)
{
	visitor.visit(*this);
}


void Indexed_tetra_set::transform(const Matrix_4d &)
{
	GTB_ERROR("not implemented yet");
}


GTB_END_NAMESPACE
