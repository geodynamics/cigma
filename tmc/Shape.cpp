#ifndef GTB_SHAPE_INCLUDED
#include "Shape.hpp"
#endif

GTB_BEGIN_NAMESPACE


Shape::~Shape()
{
}


void Shape::set_lod_hint(float hint)
{
	(void) hint;
}


float Shape::get_lod_hint() const
{
	return 0.0;
}


GTB_END_NAMESPACE
