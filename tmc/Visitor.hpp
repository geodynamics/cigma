#ifndef GTB_VISITOR_INCLUDED
#define GTB_VISITOR_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

GTB_BEGIN_NAMESPACE

class Shape;
class Indexed_face_set;
class Indexed_point_set;
class Indexed_triangle_set;
class Indexed_tetra_set;
class LOD;
class Octree;
class Shape_proxy;

class Visitor {
public:
	virtual ~Visitor() {}
	virtual void visit(Shape &) {}
	virtual void visit(Indexed_face_set &) {}
	virtual void visit(Indexed_point_set &) {}
	virtual void visit(Indexed_triangle_set &) {}
	virtual void visit(Indexed_tetra_set &) {}
	virtual void visit(LOD &) {}
	virtual void visit(Octree &) {}
	virtual void visit(Shape_proxy &) {}
};


GTB_END_NAMESPACE

#endif // GTB_VISITOR_INCLUDED
