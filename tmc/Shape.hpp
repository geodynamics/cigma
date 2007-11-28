#ifndef GTB_SHAPE_INCLUDED
#define GTB_SHAPE_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_BOUNDING_BOX_3D_INCLUDED
#include "Bounding_box_3d.hpp"
#endif

GTB_BEGIN_NAMESPACE

//class Rendering_context;
class Visitor;
class Matrix_4d;


class Shape {
public:
	virtual ~Shape();

	virtual const char *get_class_name() const = 0;
	// static const char *static_get_class_name();
	virtual const Bounding_box_3d &get_bounding_box() const = 0;
	virtual unsigned get_num_vertices() const = 0;
	virtual unsigned get_num_primitives() const = 0;
	virtual unsigned get_size_in_bytes() const = 0;
	// static Shape *read(FILE *fp);
	virtual void write(FILE *fp) const = 0;
	//virtual void render(Rendering_context &rc) = 0;
	//virtual void render_geometry(Rendering_context &rc) = 0;
	virtual void accept(Visitor &visitor) = 0;
	virtual void transform(const Matrix_4d &m) = 0;

	virtual void set_lod_hint(float hint);
	virtual float get_lod_hint() const;
};


GTB_END_NAMESPACE

#endif // GTB_SHAPE_INCLUDED
