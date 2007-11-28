#ifndef GTB_EXTERIOR_FACE_INCLUDED
#include "Exterior_face.hpp"
#endif

GTB_BEGIN_NAMESPACE


int Exterior_face::cmp(const Exterior_face &f1, const Exterior_face &f2) 
{
	if (f1.m_global_vertices[0] < f2.m_global_vertices[0]) {
		return -1;
	} else if (f1.m_global_vertices[0] > f2.m_global_vertices[0]) {
		return 1;
	} else if (f1.m_global_vertices[1] < f2.m_global_vertices[1]) {
		return -1;
	} else if (f1.m_global_vertices[1] > f2.m_global_vertices[1]) {
		return 1;
	} else if (f1.m_global_vertices[2] < f2.m_global_vertices[2]) {
		return -1;
	} else if (f1.m_global_vertices[2] > f2.m_global_vertices[2]) {
		return 1;
	}
	return 0;
}


Exterior_face::Exterior_face(const unsigned global_vertices[3],
			     unsigned global_octree_node,
			     unsigned local_tetra,
			     char local_index_on_tetra,
			     unsigned local_face)
	: m_global_octree_node(global_octree_node),
	  m_local_tetra(local_tetra),
	  m_local_index_on_tetra(local_index_on_tetra),
	  m_local_face(local_face)
{
	m_global_vertices[0] = global_vertices[0];
	m_global_vertices[1] = global_vertices[1];
	m_global_vertices[2] = global_vertices[2];
}


Exterior_face::Exterior_face(FILE *fp)
{
	read(fp);
}


void Exterior_face::read(FILE *fp) 
{
	if (fread(this, sizeof(*this), 1, fp) != 1) {
		GTB_ERROR("failed to read face");
	}
}


void Exterior_face::write(FILE *fp) const
{
 	if (fwrite(this, sizeof(*this), 1, fp) != 1) {
 		GTB_ERROR("failed to write face");
 	}
}


GTB_END_NAMESPACE
