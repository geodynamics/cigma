#ifndef GTB_POINT_3F_INCLUDED
#include "Point_3f.hpp"
#endif

// #ifndef GTB_RAY_3F_INCLUDED
// #include "Ray_3f.hpp"
// #endif

GTB_BEGIN_NAMESPACE


const Point_3f POINT_3F_ZERO(0.0, 0.0, 0.0);


Point_3f &Point_3f::scale(const Point_3f &origin, float s)
{
	Vector_3f t(origin.m_p);
	*this -= t;
	m_p[0] *= s;
	m_p[1] *= s;
	m_p[2] *= s;
	*this += t;
	return *this;
}


Point_3f &Point_3f::rotate(const Vector_3f &axis, float theta)
{
	Vector_3f v(m_p);
	v.rotate(axis, theta);
	this->reset(v.m_v[0], v.m_v[1], v.m_v[2]);
	return *this;
}


// Point_3f &Point_3f::rotate(const Ray_3f &r, float theta)
// {
// 	const Point_3f &q = r.get_origin();
// 	Vector_3f v = *this - q;
// 	v.rotate(r.get_direction(), theta);
// 	*this = q + v;
// 	return *this;
// }


Point_3f Point_3f::centroid(const std::vector<Point_3f> &v)
{
	float cx = 0.0;
	float cy = 0.0;
	float cz = 0.0;
	unsigned n = v.size();
	float n_inv = 1.0 / n;

	for (unsigned i = 0; i < n; ++i) {
		cx += v[i].m_p[0] * n_inv;
		cy += v[i].m_p[1] * n_inv;
		cz += v[i].m_p[2] * n_inv;
	}
	return Point_3f(cx, cy, cz);
}


GTB_END_NAMESPACE
