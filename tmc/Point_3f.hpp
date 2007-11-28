#ifndef GTB_POINT_3F_INCLUDED
#define GTB_POINT_3F_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_EPSILON_INCLUDED
#include "epsilon.hpp"
#endif

#ifndef GTB_MATRIX_4d_INCLUDED
#include "Matrix_4d.hpp"
#endif

#ifndef GTB_CXX_VECTOR_INCLUDED
#include <vector>
#define GTB_CXX_VECTOR_INCLUDED
#endif

#ifndef GTB_GL_HEADERS_INCLUDED
#include "gl_headers.hpp"
#endif

GTB_BEGIN_NAMESPACE

class Vector_3f;
class Ray_3f;


class Point_3f {
public:
	Point_3f();
// 	Point_3f(const Point_3f &p);
	Point_3f(float px, float py, float pz);
	Point_3f(const float p[3]);
// 	Point_3f &operator=(const Point_3f &p);

	const float &get_x() const;
	const float &get_y() const;
	const float &get_z() const;

	const float &operator[](unsigned i) const;
	float &operator[](unsigned i);

	Point_3f &set_x(float px);
	Point_3f &set_y(float py);
	Point_3f &set_z(float pz);
	Point_3f &reset(float px, float py, float pz);

	bool eps_is_equal(const Point_3f &p, float eps) const;
	bool eps_is_zero(float eps) const;

	Point_3f &operator*=(const Matrix_4d &m);
	Point_3f &operator+=(const Vector_3f &v);
	Point_3f &operator-=(const Vector_3f &v);

	friend Point_3f operator*(const Matrix_4d &m, const Point_3f &p);
	friend Point_3f operator+(const Point_3f &p, const Vector_3f &v);
	friend Vector_3f operator-(const Point_3f &p, const Point_3f &q);
	friend Point_3f operator-(const Point_3f &p, const Vector_3f &v);

	Point_3f &scale(const Point_3f &origin, float s);
	Point_3f &translate(const Vector_3f &v);
	Point_3f &translate(float dx, float dy, float dz);
	Point_3f &translate_x(float d);
	Point_3f &translate_y(float d);
	Point_3f &translate_z(float d);
	Point_3f &rotate(const Vector_3f &axis, float theta);
	Point_3f &rotate(const Ray_3f &r, float theta);
	Point_3f &transform(const Matrix_4d &m);
	Point_3f &affine_transform(const Matrix_4d &m);

	void gl_vertex() const;

	static float distance(const Point_3f &p, const Point_3f &q);

	static float squared_distance(const Point_3f &p, const Point_3f &q);

	static bool eps_are_collinear(const Point_3f &A,
				      const Point_3f &B,
				      const Point_3f &C,
				      float eps);

	static Vector_3f normal(const Point_3f &A,
				const Point_3f &B,
				const Point_3f &C);

	static Point_3f midpoint(const Point_3f &A, const Point_3f &B);

	static Point_3f centroid(const Point_3f &A,
				 const Point_3f &B,
				 const Point_3f &C);

	static Point_3f centroid(const Point_3f &A,
				 const Point_3f &B,
				 const Point_3f &C,
				 const Point_3f &D);

	static Point_3f centroid(const std::vector<Point_3f> &v);

	friend class Vector_3f;

protected:
	float m_p[3];
};


extern const Point_3f POINT_3F_ZERO;


GTB_END_NAMESPACE


#ifndef GTB_VECTOR_3F_INCLUDED
#include "Vector_3f.hpp"
#endif


GTB_BEGIN_NAMESPACE


inline Point_3f::Point_3f()
{
}


// inline Point_3f::Point_3f(const Point_3f &p)
// {
// 	m_p[0] = p.m_p[0];
// 	m_p[1] = p.m_p[1];
// 	m_p[2] = p.m_p[2];
// }


inline Point_3f::Point_3f(float px, float py, float pz)
{
	m_p[0] = px;
	m_p[1] = py;
	m_p[2] = pz;
}


inline Point_3f::Point_3f(const float p[3])
{
	m_p[0] = p[0];
	m_p[1] = p[1];
	m_p[2] = p[2];
}


// inline Point_3f &Point_3f::operator=(const Point_3f &p)
// {
// 	m_p[0] = p.m_p[0];
// 	m_p[1] = p.m_p[1];
// 	m_p[2] = p.m_p[2];
// 	return *this;
// }


inline bool Point_3f::eps_is_equal(const Point_3f &p, float eps) const
{
	return (gtb::eps_is_equal(m_p[0], p.m_p[0], eps) &&
		gtb::eps_is_equal(m_p[1], p.m_p[1], eps) &&
		gtb::eps_is_equal(m_p[2], p.m_p[2], eps));
}


inline bool Point_3f::eps_is_zero(float eps) const
{
	return (gtb::eps_is_zero(m_p[0], eps) &&
		gtb::eps_is_zero(m_p[1], eps) &&
		gtb::eps_is_zero(m_p[2], eps));
}


inline const float &Point_3f::get_x() const
{
	return m_p[0];
}


inline const float &Point_3f::get_y() const
{
	return m_p[1];
}


inline const float &Point_3f::get_z() const
{
	return m_p[2];
}


inline const float &Point_3f::operator[](unsigned i) const
{
	GTB_REQUIRE(i < 3);
	return m_p[i];
}


inline float &Point_3f::operator[](unsigned i)
{
	GTB_REQUIRE(i < 3);
	return m_p[i];
}


inline Point_3f &Point_3f::set_x(float px)
{
	m_p[0] = px;
	return *this;
}


inline Point_3f &Point_3f::set_y(float py)
{
	m_p[1] = py;
	return *this;
}


inline Point_3f &Point_3f::set_z(float pz)
{
	m_p[2] = pz;
	return *this;
}


inline Point_3f &Point_3f::reset(float px, float py, float pz)
{
	m_p[0] = px;
	m_p[1] = py;
	m_p[2] = pz;
	return *this;
}


inline Point_3f &Point_3f::operator*=(const Matrix_4d &m)
{
	float w = m_p[0] * m(3, 0) 
		+ m_p[1] * m(3, 1) 
		+ m_p[2] * m(3, 2) 
		+ m(3, 3);

	float wi = 1.0 / w;

	return reset((m_p[0] * m(0, 0) 
		      + m_p[1] * m(0, 1) 
		      + m_p[2] * m(0, 2) 
		      + m(0, 3)) * wi,

		     (m_p[0] * m(1, 0) 
		      + m_p[1] * m(1, 1)
		      + m_p[2] * m(1, 2) 
		      + m(1, 3)) * wi,

		     (m_p[0] * m(2, 0)
		      + m_p[1] * m(2, 1)
		      + m_p[2] * m(2, 2)
		      + m(2, 3)) * wi);
}


inline Point_3f &Point_3f::operator+=(const Vector_3f &v)
{
	m_p[0] += v.m_v[0];
	m_p[1] += v.m_v[1];
	m_p[2] += v.m_v[2];
	return *this;
}


inline Point_3f &Point_3f::operator-=(const Vector_3f &v)
{
	m_p[0] -= v.m_v[0];
	m_p[1] -= v.m_v[1];
	m_p[2] -= v.m_v[2];
	return *this;
}


inline Point_3f operator*(const Matrix_4d &m, const Point_3f &p)
{
#if 0
	float w = p.m_p[0] * m(3, 0)
		+ p.m_p[1] * m(3, 1)
		+ p.m_p[2] * m(3, 2)
		+ m(3, 3);

	float wi = 1.0 / w;
#endif
	float wi = 1;

	return Point_3f((p.m_p[0] * m(0, 0)
			 + p.m_p[1] * m(0, 1)
			 + p.m_p[2] * m(0, 2)
			 + m(0, 3)) * wi,

			(p.m_p[0] * m(1, 0)
			 + p.m_p[1] * m(1, 1)
			 + p.m_p[2] * m(1, 2)
			 + m(1, 3)) * wi,

			(p.m_p[0] * m(2, 0)
			 + p.m_p[1] * m(2, 1)
			 + p.m_p[2] * m(2, 2)
			 + m(2, 3)) * wi);
}


inline Point_3f operator+(const Point_3f &p, const Vector_3f &v)
{
	return Point_3f(p.m_p[0] + v.m_v[0],
			p.m_p[1] + v.m_v[1],
			p.m_p[2] + v.m_v[2]);
}


inline Vector_3f operator-(const Point_3f &p, const Point_3f &q)
{
	return Vector_3f(p.m_p[0] - q.m_p[0],
			 p.m_p[1] - q.m_p[1],
			 p.m_p[2] - q.m_p[2]);
}


inline Point_3f operator-(const Point_3f &p, const Vector_3f &v)
{
	return Point_3f(p.m_p[0] - v.m_v[0],
			p.m_p[1] - v.m_v[1],
			p.m_p[2] - v.m_v[2]);
}


inline Point_3f &Point_3f::translate(const Vector_3f &t)
{
	return *this += t;
}


inline Point_3f &Point_3f::translate(float dx, float dy, float dz)
{
	m_p[0] += dx;
	m_p[1] += dy;
	m_p[2] += dz;
	return *this;
}


inline Point_3f &Point_3f::translate_x(float d)
{
	m_p[0] += d;
	return *this;
}


inline Point_3f &Point_3f::translate_y(float d)
{
	m_p[1] += d;
	return *this;
}


inline Point_3f &Point_3f::translate_z(float d)
{
	m_p[2] += d;
	return *this;
}


inline void Point_3f::gl_vertex() const
{
	glVertex3fv(m_p);
}


inline float Point_3f::distance(const Point_3f &p, const Point_3f &q)
{
	return (p - q).length();
}


inline float Point_3f::squared_distance(const Point_3f &p, const Point_3f &q)
{
	return (p - q).squared_length();
}


inline bool Point_3f::eps_are_collinear(const Point_3f &A,
					const Point_3f &B,
					const Point_3f &C,
					float eps)
{
	return (B - A).cross(C - A).eps_is_zero(eps);
}


inline Vector_3f Point_3f::normal(const Point_3f &A,
				  const Point_3f &B,
				  const Point_3f &C)
{
	return (B - A).cross(C - A).normalize();
}


inline Point_3f Point_3f::midpoint(const Point_3f &A, const Point_3f &B)
{
	return Point_3f(0.5 * (A.m_p[0] + B.m_p[0]),
			0.5 * (A.m_p[1] + B.m_p[1]),
			0.5 * (A.m_p[2] + B.m_p[2]));
}


inline Point_3f Point_3f::centroid(const Point_3f &A,
				   const Point_3f &B,
				   const Point_3f &C)
{
	return Point_3f((A.m_p[0] + B.m_p[0] + C.m_p[0]) / 3.0,
			(A.m_p[1] + B.m_p[1] + C.m_p[1]) / 3.0,
			(A.m_p[2] + B.m_p[2] + C.m_p[2]) / 3.0);
}


inline Point_3f Point_3f::centroid(const Point_3f &A,
				   const Point_3f &B,
				   const Point_3f &C,
				   const Point_3f &D)
{
	return Point_3f((A.m_p[0] + B.m_p[0] + C.m_p[0] + D.m_p[0]) / 4.0,
			(A.m_p[1] + B.m_p[1] + C.m_p[1] + D.m_p[1]) / 4.0,
			(A.m_p[2] + B.m_p[2] + C.m_p[2] + D.m_p[2]) / 4.0);
}


inline Point_3f &Point_3f::transform(const Matrix_4d &m)
{
	return *this *= m;
}


inline Point_3f &Point_3f::affine_transform(const Matrix_4d &m)
{
	return reset(m_p[0] * m(0, 0)
		     + m_p[1] * m(0, 1)
		     + m_p[2] * m(0, 2)
		     + m(0, 3),

		     m_p[0] * m(1, 0)
		     + m_p[1] * m(1, 1)
		     + m_p[2] * m(1, 2)
		     + m(1, 3),

		     m_p[0] * m(2, 0)
		     + m_p[1] * m(2, 1)
		     + m_p[2] * m(2, 2)
		     + m(2, 3));
}


GTB_END_NAMESPACE

#endif // GTB_POINT_3F_INCLUDED
