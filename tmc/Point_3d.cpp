#ifndef GTB_POINT_3D_INCLUDED
#include "Point_3d.hpp"
#endif

// #ifndef GTB_RAY_3D_INCLUDED
// #include "Ray_3d.hpp"
// #endif

GTB_BEGIN_NAMESPACE


const Point_3d POINT_3D_ZERO(0.0, 0.0, 0.0);


Point_3d &Point_3d::scale(const Point_3d &origin, double s)
{
	Vector_3d t(origin.m_p);
	*this -= t;
	m_p[0] *= s;
	m_p[1] *= s;
	m_p[2] *= s;
	*this += t;
	return *this;
}


Point_3d &Point_3d::rotate(const Vector_3d &axis, double theta)
{
	Vector_3d v(m_p);
	v.rotate(axis, theta);
	this->reset(v.m_v[0], v.m_v[1], v.m_v[2]);
	return *this;
}


// Point_3d &Point_3d::rotate(const Ray_3d &r, double theta)
// {
// 	const Point_3d &q = r.get_origin();
// 	Vector_3d v = *this - q;
// 	v.rotate(r.get_direction(), theta);
// 	*this = q + v;
// 	return *this;
// }


Point_3d Point_3d::centroid(const std::vector<Point_3d> &v)
{
	double cx = 0.0;
	double cy = 0.0;
	double cz = 0.0;
	unsigned n = v.size();
	double n_inv = 1.0 / n;

	for (unsigned i = 0; i < n; ++i) {
		cx += v[i].m_p[0] * n_inv;
		cy += v[i].m_p[1] * n_inv;
		cz += v[i].m_p[2] * n_inv;
	}
	return Point_3d(cx, cy, cz);
}


GTB_END_NAMESPACE
