#ifndef GTB_TETRAHEDRON_INCLUDED
#define GTB_TETRAHEDRON_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_CXX_STRING_INCLUDED
#include <string>
#define GTB_CXX_STRING_INCLUDED
#endif

#ifndef GTB_ASSERTIONS_INCLUDED
#include "assertions.hpp"
#endif

#ifndef GTB_C_ASSERT_INCLUDED
#include <assert.h>
#define GTB_C_ASSERT_INCLUDED
#endif

GTB_BEGIN_NAMESPACE


class Tetrahedron {
public:
	Tetrahedron();
	Tetrahedron(unsigned v0, unsigned v1, unsigned v2, unsigned v3);
	~Tetrahedron();

	typedef enum {
		FT_UNDEFINED,
		FT_INBOUND,
		FT_EXTERIOR_INBOUND,
		FT_OUTBOUND,
		FT_EXTERIOR_OUTBOUND
	} face_type_t;

	typedef enum {
		TT_NORMAL,
		TT_DEGENERATE,
		TT_VIRTUAL
	} tetra_type_t;

	unsigned get_vertex(unsigned i) const;

	unsigned get_face(unsigned i) const;
	void set_face(unsigned i, unsigned f);

	int get_neighbor(unsigned i) const;
	void set_neighbor(unsigned i, int n);

	unsigned get_neighbor_octree_node(unsigned i) const;
 	void set_neighbor_octree_node(unsigned i, unsigned on);

	face_type_t get_face_type(unsigned f) const;
	void set_face_type(unsigned f, face_type_t t);
	std::string get_face_type_name(unsigned f) const;

	bool is_face_exterior(unsigned f) const;
	bool is_exterior() const;
	void get_face_indices(unsigned f, unsigned v[4]) const;

	unsigned get_inbound_count() const;
	void set_inbound_count(unsigned n);
	void inc_inbound_count();
	void dec_inbound_count();

	bool is_visited() const;
	void set_visited(bool visited);

	unsigned get_octree_node() const;
	void set_octree_node(unsigned node);

	tetra_type_t get_type() const;
	bool are_vertices_valid() const;

private:
	unsigned m_vertices[4];
	unsigned m_faces[4];
	union {
		int m_neighbors[4];
		int m_centroid[4];
	};
	unsigned m_neighbor_octree_node[4];
	face_type_t m_face_types[4];
	tetra_type_t m_type;
	unsigned m_inbound_count;
	bool m_visited;
	unsigned m_octree_node;
};


inline unsigned Tetrahedron::get_vertex(unsigned i) const
{
	assert(i < 4);
	return m_vertices[i];
}


inline unsigned Tetrahedron::get_face(unsigned i) const
{
	assert(i < 4);
	return m_faces[i];
}


inline void Tetrahedron::set_face(unsigned i, unsigned f)
{
	assert(i < 4);
	m_faces[i] = f;
}


inline int Tetrahedron::get_neighbor(unsigned i) const
{
	assert(i < 4);
	return m_neighbors[i];
}


inline void Tetrahedron::set_neighbor(unsigned i, int n)
{
	assert(i < 4);
	m_neighbors[i] = n;
}


inline unsigned Tetrahedron::get_neighbor_octree_node(unsigned i) const
{
	assert(i < 4);
	return m_neighbor_octree_node[i];
}


inline void Tetrahedron::set_neighbor_octree_node(unsigned i, unsigned on)
{
	assert(i < 4);
	m_neighbor_octree_node[i] = on;
}


inline unsigned Tetrahedron::get_octree_node() const
{
	return m_octree_node;
}


inline void Tetrahedron::set_octree_node(unsigned node)
{
	m_octree_node = node;
}


inline Tetrahedron::face_type_t Tetrahedron::get_face_type(unsigned f) const
{
	assert(f < 4);
	return m_face_types[f];
}


inline void Tetrahedron::set_face_type(unsigned f, face_type_t t)
{
	assert(f < 4);
	m_face_types[f] = t;
}


inline std::string Tetrahedron::get_face_type_name(unsigned f) const
{
	std::string s;
	assert(f < 4);
	switch (m_face_types[f]) {
	case FT_UNDEFINED:
		s = "undefined";
		break;
	case FT_INBOUND:
		s = "inbound";
		break;
	case FT_EXTERIOR_INBOUND:
		s = "exterior_inbound";
		break;
	case FT_OUTBOUND:
		s = "outbound";
		break;
	case FT_EXTERIOR_OUTBOUND:
		s = "exterior_outbound";
		break;
	default:
		GTB_ERROR("invalid face type");
	}
	return s;
}


inline bool Tetrahedron::is_face_exterior(unsigned f) const
{
	assert(f < 4);
	return m_neighbors[f] <= 0;
}


inline bool Tetrahedron::is_exterior() const
{
	return (is_face_exterior(0) ||
		is_face_exterior(1) ||
		is_face_exterior(2) ||
		is_face_exterior(3));
}


inline void Tetrahedron::get_face_indices(unsigned f, unsigned v[4]) const
{
	assert(f < 4);

	switch (f) {
	case 0:
		v[0] = m_vertices[1];
		v[1] = m_vertices[2];
		v[2] = m_vertices[3];
		v[3] = m_vertices[0];
		break;
	case 1:
		v[0] = m_vertices[0];
		v[1] = m_vertices[2];
		v[2] = m_vertices[3];
		v[3] = m_vertices[1];
		break;
	case 2:
		v[0] = m_vertices[0];
		v[1] = m_vertices[1];
		v[2] = m_vertices[3];
		v[3] = m_vertices[2];
		break;
	case 3:
		v[0] = m_vertices[0];
		v[1] = m_vertices[1];
		v[2] = m_vertices[2];
		v[3] = m_vertices[3];
		break;
	default:
		GTB_ERROR("should never get here");
		break;
	}
}


inline unsigned Tetrahedron::get_inbound_count() const
{
	return m_inbound_count;
}


inline void Tetrahedron::set_inbound_count(unsigned n)
{
	m_inbound_count = n;
}


inline void Tetrahedron::inc_inbound_count()
{
	m_inbound_count++;
}


inline void Tetrahedron::dec_inbound_count()
{
	GTB_REQUIRE(m_inbound_count > 0);
	m_inbound_count--;
}


inline bool Tetrahedron::is_visited() const
{
	return m_visited;
}


inline void Tetrahedron::set_visited(bool visited)
{
	m_visited = visited;
}


inline Tetrahedron::tetra_type_t Tetrahedron::get_type() const
{
	return m_type;
}


GTB_END_NAMESPACE

#endif // GTB_TETRAHEDRON_INCLUDED
