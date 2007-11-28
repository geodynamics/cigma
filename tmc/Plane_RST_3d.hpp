#ifndef GTB_PLANE_RST_3D_INCLUDED
#define GTB_PLANE_RST_3D_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

GTB_BEGIN_NAMESPACE


// This class represents a plane in 3D using two angles and a distance
// from the origin
class Plane_RST_3d {
public:
	Plane_RST_3d();
	Plane_RST_3d(double r, double s, double t);

	const double &get_r() const;
	const double &get_s() const;
	const double &get_t() const;

protected:
	double m_r, m_s;	// Angles
	double m_t;		// Distance from the origin
};


inline Plane_RST_3d::Plane_RST_3d()
{
}


inline Plane_RST_3d::Plane_RST_3d(double r, double s, double t)
	: m_r(r),
	  m_s(s),
	  m_t(t)
{
}


inline const double &Plane_RST_3d::get_r() const
{
	return m_r;
}


inline const double &Plane_RST_3d::get_s() const
{
	return m_s;
}


inline const double &Plane_RST_3d::get_t() const
{
	return m_t;
}


GTB_END_NAMESPACE

#endif // GTB_PLANE_RST_3D_INCLUDED
