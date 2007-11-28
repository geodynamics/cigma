#ifndef GTB_TETRAHEDRON_INCLUDED
#include "Tetrahedron.hpp"
#endif

#ifndef GTB_CXX_VECTOR_INCLUDED
#include <vector>
#define GTB_CXX_VECTOR_INCLUDED
#endif

#ifndef GTB_CXX_ALGORITHM_INCLUDED
#include <algorithm>
#define GTB_CXX_ALGORITHM_INCLUDED
#endif

GTB_BEGIN_NAMESPACE


Tetrahedron::Tetrahedron()
	: m_type(TT_NORMAL),
	  m_inbound_count(0),
	  m_visited(false)
{
	m_vertices[0] = 0;
	m_vertices[1] = 0;
	m_vertices[2] = 0;
	m_vertices[3] = 0;

	m_faces[0] = 0;
	m_faces[1] = 0;
	m_faces[2] = 0;
	m_faces[3] = 0;

	m_neighbors[0] = 0;
	m_neighbors[1] = 0;
	m_neighbors[2] = 0;
	m_neighbors[3] = 0;

	m_face_types[0] = FT_UNDEFINED;
	m_face_types[1] = FT_UNDEFINED;
	m_face_types[2] = FT_UNDEFINED;
	m_face_types[3] = FT_UNDEFINED;
}


Tetrahedron::Tetrahedron(unsigned v0, unsigned v1, unsigned v2, unsigned v3)
	: m_type(TT_NORMAL),
	  m_inbound_count(0),
	  m_visited(false)
{
	m_vertices[0] = v0;
	m_vertices[1] = v1;
	m_vertices[2] = v2;
	m_vertices[3] = v3;

	m_faces[0] = 0;
	m_faces[1] = 0;
	m_faces[2] = 0;
	m_faces[3] = 0;

	m_neighbors[0] = 0;
	m_neighbors[1] = 0;
	m_neighbors[2] = 0;
	m_neighbors[3] = 0;

	m_face_types[0] = FT_UNDEFINED;
	m_face_types[1] = FT_UNDEFINED;
	m_face_types[2] = FT_UNDEFINED;
	m_face_types[3] = FT_UNDEFINED;
}


Tetrahedron::~Tetrahedron()
{
}


bool Tetrahedron::are_vertices_valid() const
{
	std::vector<unsigned> v(4);
	v[0] = get_vertex(0);
	v[1] = get_vertex(1);
	v[2] = get_vertex(2);
	v[3] = get_vertex(3);
	std::sort(v.begin(), v.end());
	for (unsigned j = 0; j < 3; ++j) {
		if (v[j] == v[j + 1]) {
			return false;
		}
	}
	return true;
}


GTB_END_NAMESPACE
