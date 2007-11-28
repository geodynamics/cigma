#ifndef GTB_EXTERIOR_FACE_INCLUDED
#define GTB_EXTERIOR_FACE_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_ASSERTIONS_INCLUDED
#include "assertions.hpp"
#endif

#ifndef GTB_C_STDIO_INCLUDED
#include <stdio.h>
#define GTB_C_STDIO_INCLUDED
#endif

GTB_BEGIN_NAMESPACE


class Exterior_face  {
public:
	Exterior_face(const unsigned global_vertices[3],
		      unsigned global_octree_node,
		      unsigned local_tetra,
		      char local_index_on_tetra,
		      unsigned local_face);

	Exterior_face(FILE *fp);

	bool operator==(const Exterior_face &f) const;
	static int cmp(const Exterior_face &f1, 
		       const Exterior_face &f2);
	void write(FILE *fp) const;
	void read(FILE *fp);

	void get_global_vertices(unsigned v[3]) const;
	unsigned get_global_vertex(unsigned v) const;
	const unsigned &get_global_octree_node() const;
	const unsigned &get_local_tetra() const;
	const char &get_local_index_on_tetra() const;
	const unsigned &get_local_face() const;

protected:
	unsigned m_global_vertices[3];
	unsigned m_global_octree_node;
	unsigned m_local_tetra;
	char m_local_index_on_tetra;
	unsigned m_local_face;
};


inline bool Exterior_face::operator==(const Exterior_face &f) const 
{
	return cmp(*this, f) == 0;
}


inline void Exterior_face::get_global_vertices(	unsigned v[3]) const
{
	v[0] = m_global_vertices[0];
	v[1] = m_global_vertices[1];
	v[2] = m_global_vertices[2];
}


inline unsigned Exterior_face::get_global_vertex(unsigned v) const
{
	GTB_REQUIRE(v < 3);
	return m_global_vertices[v];
}


inline const unsigned &Exterior_face::get_global_octree_node() const
{
	return m_global_octree_node;
}


inline const unsigned &Exterior_face::get_local_tetra() const
{
	return m_local_tetra;
}


inline const char &Exterior_face::get_local_index_on_tetra() const
{
	return m_local_index_on_tetra;
}


inline const unsigned &Exterior_face::get_local_face() const
{
	return m_local_face;
}


GTB_END_NAMESPACE

#endif // GTB_EXTERIOR_FACE_INCLUDED
