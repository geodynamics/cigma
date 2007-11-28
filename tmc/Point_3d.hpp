#ifndef GTB_POINT_3D_INCLUDED
#define GTB_POINT_3D_INCLUDED

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

class Vector_3d;
class Ray_3d;


class Point_3d {
public:
	Point_3d();
	Point_3d(const Point_3d &p);
	Point_3d(double px, double py, double pz);
	Point_3d(const double p[3]);
	Point_3d &operator=(const Point_3d &p);

	const double &get_x() const;
	const double &get_y() const;
	const double &get_z() const;

	const double &operator[](unsigned i) const;
	double &operator[](unsigned i);

	Point_3d &set_x(double px);
	Point_3d &set_y(double py);
	Point_3d &set_z(double pz);
	Point_3d &reset(double px, double py, double pz);

	bool eps_is_equal(const Point_3d &p, double eps) const;
	bool eps_is_zero(double eps) const;

	Point_3d &operator*=(const Matrix_4d &m);
	Point_3d &operator+=(const Vector_3d &v);
	Point_3d &operator-=(const Vector_3d &v);

	friend Point_3d operator*(const Matrix_4d &m, const Point_3d &p);
	friend Point_3d operator+(const Point_3d &p, const Vector_3d &v);
	friend Vector_3d operator-(const Point_3d &p, const Point_3d &q);
	friend Point_3d operator-(const Point_3d &p, const Vector_3d &v);

	Point_3d &scale(const Point_3d &origin, double s);
	Point_3d &translate(const Vector_3d &v);
	Point_3d &translate(double dx, double dy, double dz);
	Point_3d &translate_x(double d);
	Point_3d &translate_y(double d);
	Point_3d &translate_z(double d);
	Point_3d &rotate(const Vector_3d &axis, double theta);
	Point_3d &rotate(const Ray_3d &r, double theta);
	Point_3d &transform(const Matrix_4d &m);
	Point_3d &affine_transform(const Matrix_4d &m);

	void gl_vertex() const;

	static double distance(const Point_3d &p, const Point_3d &q);

	static double squared_distance(const Point_3d &p, const Point_3d &q);

	static bool eps_are_collinear(const Point_3d &A,
				      const Point_3d &B,
				      const Point_3d &C,
				      double eps);

	static Vector_3d normal(const Point_3d &A,
				const Point_3d &B,
				const Point_3d &C);

	static Point_3d midpoint(const Point_3d &A, const Point_3d &B);

	static Point_3d centroid(const Point_3d &A,
				 const Point_3d &B,
				 const Point_3d &C);

	static Point_3d centroid(const Point_3d &A,
				 const Point_3d &B,
				 const Point_3d &C,
				 const Point_3d &D);

	static Point_3d centroid(const std::vector<Point_3d> &v);

	friend class Vector_3d;

protected:
	double m_p[3];
};


extern const Point_3d POINT_3D_ZERO;


GTB_END_NAMESPACE


#ifndef GTB_VECTOR_3D_INCLUDED
#include "Vector_3d.hpp"
#endif


GTB_BEGIN_NAMESPACE


inline Point_3d::Point_3d()
{
}


inline Point_3d::Point_3d(const Point_3d &p)
{
	m_p[0] = p.m_p[0];
	m_p[1] = p.m_p[1];
	m_p[2] = p.m_p[2];
}


inline Point_3d::Point_3d(double px, double py, double pz)
{
	m_p[0] = px;
	m_p[1] = py;
	m_p[2] = pz;
}


inline Point_3d::Point_3d(const double p[3])
{
	m_p[0] = p[0];
	m_p[1] = p[1];
	m_p[2] = p[2];
}


inline Point_3d &Point_3d::operator=(const Point_3d &p)
{
	m_p[0] = p.m_p[0];
	m_p[1] = p.m_p[1];
	m_p[2] = p.m_p[2];
	return *this;
}


inline bool Point_3d::eps_is_equal(const Point_3d &p, double eps) const
{
	return (gtb::eps_is_equal(m_p[0], p.m_p[0], eps) &&
		gtb::eps_is_equal(m_p[1], p.m_p[1], eps) &&
		gtb::eps_is_equal(m_p[2], p.m_p[2], eps));
}


inline bool Point_3d::eps_is_zero(double eps) const
{
	return (gtb::eps_is_zero(m_p[0], eps) &&
		gtb::eps_is_zero(m_p[1], eps) &&
		gtb::eps_is_zero(m_p[2], eps));
}


inline const double &Point_3d::get_x() const
{
	return m_p[0];
}


inline const double &Point_3d::get_y() const
{
	return m_p[1];
}


inline const double &Point_3d::get_z() const
{
	return m_p[2];
}


inline const double &Point_3d::operator[](unsigned i) const
{
	GTB_REQUIRE(i < 3);
	return m_p[i];
}


inline double &Point_3d::operator[](unsigned i)
{
	GTB_REQUIRE(i < 3);
	return m_p[i];
}


inline Point_3d &Point_3d::set_x(double px)
{
	m_p[0] = px;
	return *this;
}


inline Point_3d &Point_3d::set_y(double py)
{
	m_p[1] = py;
	return *this;
}


inline Point_3d &Point_3d::set_z(double pz)
{
	m_p[2] = pz;
	return *this;
}


inline Point_3d &Point_3d::reset(double px, double py, double pz)
{
	m_p[0] = px;
	m_p[1] = py;
	m_p[2] = pz;
	return *this;
}


inline Point_3d &Point_3d::operator*=(const Matrix_4d &m)
{
	double w = m_p[0] * m(3, 0) 
		+ m_p[1] * m(3, 1) 
		+ m_p[2] * m(3, 2) 
		+ m(3, 3);

	double wi = 1.0 / w;

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


inline Point_3d &Point_3d::operator+=(const Vector_3d &v)
{
	m_p[0] += v.m_v[0];
	m_p[1] += v.m_v[1];
	m_p[2] += v.m_v[2];
	return *this;
}


inline Point_3d &Point_3d::operator-=(const Vector_3d &v)
{
	m_p[0] -= v.m_v[0];
	m_p[1] -= v.m_v[1];
	m_p[2] -= v.m_v[2];
	return *this;
}


inline Point_3d operator*(const Matrix_4d &m, const Point_3d &p)
{
	double w = p.m_p[0] * m(3, 0)
		+ p.m_p[1] * m(3, 1)
		+ p.m_p[2] * m(3, 2)
		+ m(3, 3);

	double wi = 1.0 / w;

	return Point_3d((p.m_p[0] * m(0, 0)
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


inline Point_3d operator+(const Point_3d &p, const Vector_3d &v)
{
	return Point_3d(p.m_p[0] + v.m_v[0],
			p.m_p[1] + v.m_v[1],
			p.m_p[2] + v.m_v[2]);
}


inline Vector_3d operator-(const Point_3d &p, const Point_3d &q)
{
	return Vector_3d(p.m_p[0] - q.m_p[0],
			 p.m_p[1] - q.m_p[1],
			 p.m_p[2] - q.m_p[2]);
}


inline Point_3d operator-(const Point_3d &p, const Vector_3d &v)
{
	return Point_3d(p.m_p[0] - v.m_v[0],
			p.m_p[1] - v.m_v[1],
			p.m_p[2] - v.m_v[2]);
}


inline Point_3d &Point_3d::translate(const Vector_3d &t)
{
	return *this += t;
}


inline Point_3d &Point_3d::translate(double dx, double dy, double dz)
{
	m_p[0] += dx;
	m_p[1] += dy;
	m_p[2] += dz;
	return *this;
}


inline Point_3d &Point_3d::translate_x(double d)
{
	m_p[0] += d;
	return *this;
}


inline Point_3d &Point_3d::translate_y(double d)
{
	m_p[1] += d;
	return *this;
}


inline Point_3d &Point_3d::translate_z(double d)
{
	m_p[2] += d;
	return *this;
}


inline void Point_3d::gl_vertex() const
{
	glVertex3dv(m_p);
}


inline double Point_3d::distance(const Point_3d &p, const Point_3d &q)
{
	return (p - q).length();
}


inline double Point_3d::squared_distance(const Point_3d &p, const Point_3d &q)
{
	return (p - q).squared_length();
}


inline bool Point_3d::eps_are_collinear(const Point_3d &A,
					const Point_3d &B,
					const Point_3d &C,
					double eps)
{
	return (B - A).cross(C - A).eps_is_zero(eps);
}


inline Vector_3d Point_3d::normal(const Point_3d &A,
				  const Point_3d &B,
				  const Point_3d &C)
{
	return (B - A).cross(C - A).normalize();
}


inline Point_3d Point_3d::midpoint(const Point_3d &A, const Point_3d &B)
{
	return Point_3d(0.5 * (A.m_p[0] + B.m_p[0]),
			0.5 * (A.m_p[1] + B.m_p[1]),
			0.5 * (A.m_p[2] + B.m_p[2]));
}


inline Point_3d Point_3d::centroid(const Point_3d &A,
				   const Point_3d &B,
				   const Point_3d &C)
{
	return Point_3d((A.m_p[0] + B.m_p[0] + C.m_p[0]) / 3.0,
			(A.m_p[1] + B.m_p[1] + C.m_p[1]) / 3.0,
			(A.m_p[2] + B.m_p[2] + C.m_p[2]) / 3.0);
}


inline Point_3d Point_3d::centroid(const Point_3d &A,
				   const Point_3d &B,
				   const Point_3d &C,
				   const Point_3d &D)
{
	return Point_3d((A.m_p[0] + B.m_p[0] + C.m_p[0] + D.m_p[0]) / 4.0,
			(A.m_p[1] + B.m_p[1] + C.m_p[1] + D.m_p[1]) / 4.0,
			(A.m_p[2] + B.m_p[2] + C.m_p[2] + D.m_p[2]) / 4.0);
}


inline Point_3d &Point_3d::transform(const Matrix_4d &m)
{
	return *this *= m;
}


inline Point_3d &Point_3d::affine_transform(const Matrix_4d &m)
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

#endif // GTB_POINT_3D_INCLUDED
