#ifndef GTB_VECTOR_3D_INCLUDED
#define GTB_VECTOR_3D_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_GL_HEADERS_INCLUDED
#include "gl_headers.hpp"
#endif

GTB_BEGIN_NAMESPACE

class Point_3d;
class Matrix_4d;


class Vector_3d {
public:
	Vector_3d();
	Vector_3d(const Vector_3d &v);
	explicit Vector_3d(const Point_3d &p);
	Vector_3d(double vx, double vy, double vz);
	explicit Vector_3d(const double v[3]);
	explicit Vector_3d(const double v);
	Vector_3d &operator=(const Vector_3d &v);

	const double &get_x() const;
	const double &get_y() const;
	const double &get_z() const;

	const double &operator[](unsigned i) const;
	double &operator[](unsigned i);

	Vector_3d &set_x(double vx);
	Vector_3d &set_y(double vy);
	Vector_3d &set_z(double vz);
	Vector_3d &reset(double vx, double vy, double vz);

	bool eps_is_equal(const Vector_3d &v, double eps) const;
	bool eps_is_zero(double eps) const;
	bool eps_is_normalized(double eps) const;

	Vector_3d operator-() const;
	Vector_3d &operator*=(const Matrix_4d &m);
	Vector_3d &operator+=(const Vector_3d &v);
	Vector_3d &operator-=(const Vector_3d &v);
	Vector_3d &operator*=(double a);
	Vector_3d &operator/=(double a);

	double operator*(const Vector_3d &v) const;
	double operator*(const Point_3d &p) const;

	friend Vector_3d operator+(const Vector_3d &u, const Vector_3d &v);
	friend Point_3d operator+(const Point_3d &p, const Vector_3d &v);
	friend Vector_3d operator-(const Vector_3d &u, const Vector_3d &v);
	friend Point_3d operator-(const Point_3d &p, const Vector_3d &v);
	friend Vector_3d operator*(double a, const Vector_3d &v);
	friend Vector_3d operator*(const Vector_3d &v, double a);
	friend Vector_3d operator*(const Matrix_4d &m, const Vector_3d &v);
	friend Vector_3d operator/(const Vector_3d &v, double a);

	double dot(const Vector_3d &v) const;
	double dot(const Point_3d &p) const;
	Vector_3d cross(const Vector_3d &v) const;
	double length() const;
	double squared_length() const;
	Vector_3d &normalize();
	Vector_3d &normalize(double &len);
	Vector_3d &scale(double a);
	Vector_3d &rotate(const Vector_3d &axis, double theta);
	Vector_3d &transform(const Matrix_4d &m);
	Vector_3d &affine_transform(const Matrix_4d &m);
	void flip();

	void gl_normal() const;

	static Matrix_4d rotation(const Vector_3d &from, const Vector_3d &to);

	friend class Point_3d;
	friend void swap(Vector_3d &a, Vector_3d &b);

protected:
	double m_v[3];
};


extern const Vector_3d VECTOR_3D_ZERO;
extern const Vector_3d VECTOR_3D_POSITIVE_X;
extern const Vector_3d VECTOR_3D_NEGATIVE_X;
extern const Vector_3d VECTOR_3D_POSITIVE_Y;
extern const Vector_3d VECTOR_3D_NEGATIVE_Y;
extern const Vector_3d VECTOR_3D_POSITIVE_Z;
extern const Vector_3d VECTOR_3D_NEGATIVE_Z;


GTB_END_NAMESPACE


#ifndef GTB_POINT_3D_INCLUDED
#include "Point_3d.hpp"
#endif


GTB_BEGIN_NAMESPACE


inline Vector_3d::Vector_3d()
{
}


inline Vector_3d::Vector_3d(const Vector_3d &v)
{
	m_v[0] = v.m_v[0];
	m_v[1] = v.m_v[1];
	m_v[2] = v.m_v[2];
}


inline Vector_3d::Vector_3d(double vx, double vy, double vz)
{
	m_v[0] = vx;
	m_v[1] = vy;
	m_v[2] = vz;
}


inline Vector_3d::Vector_3d(const double v[3])
{
	m_v[0] = v[0];
	m_v[1] = v[1];
	m_v[2] = v[2];
}


inline Vector_3d::Vector_3d(const double v) 
{
	m_v[0] = v;
	m_v[1] = v;
	m_v[2] = v;
}


inline Vector_3d::Vector_3d(const Point_3d &p)
{
	m_v[0] = p.m_p[0];
	m_v[1] = p.m_p[1];
	m_v[2] = p.m_p[2];
}


inline Vector_3d &Vector_3d::operator=(const Vector_3d &v)
{
	m_v[0] = v.m_v[0];
	m_v[1] = v.m_v[1];
	m_v[2] = v.m_v[2];
	return *this;
}


inline const double &Vector_3d::get_x() const
{
	return m_v[0];
}


inline const double &Vector_3d::get_y() const
{
	return m_v[1];
}


inline const double &Vector_3d::get_z() const
{
	return m_v[2];
}


inline const double &Vector_3d::operator[](unsigned i) const
{
	GTB_REQUIRE(i < 3);
	return m_v[i];
}


inline double &Vector_3d::operator[](unsigned i)
{
	GTB_REQUIRE(i < 3);
	return m_v[i];
}


inline Vector_3d &Vector_3d::set_x(double vx)
{
	m_v[0] = vx;
	return *this;
}


inline Vector_3d &Vector_3d::set_y(double vy)
{
	m_v[1] = vy;
	return *this;
}


inline Vector_3d &Vector_3d::set_z(double vz)
{
	m_v[2] = vz;
	return *this;
}


inline Vector_3d &Vector_3d::reset(double vx, double vy, double vz)
{
	m_v[0] = vx;
	m_v[1] = vy;
	m_v[2] = vz;
	return *this;
}


inline bool Vector_3d::eps_is_equal(const Vector_3d &v, double eps) const
{
	return (gtb::eps_is_equal(m_v[0], v.m_v[0], eps) &&
		gtb::eps_is_equal(m_v[1], v.m_v[1], eps) &&
		gtb::eps_is_equal(m_v[2], v.m_v[2], eps));
}


inline bool Vector_3d::eps_is_zero(double eps) const
{
	return (gtb::eps_is_zero(m_v[0], eps) &&
		gtb::eps_is_zero(m_v[1], eps) &&
		gtb::eps_is_zero(m_v[2], eps));
}


inline Vector_3d Vector_3d::operator-() const
{
	return Vector_3d(-m_v[0],
			 -m_v[1],
			 -m_v[2]);
}


inline Vector_3d &Vector_3d::operator+=(const Vector_3d &v)
{
	m_v[0] += v.m_v[0];
	m_v[1] += v.m_v[1];
	m_v[2] += v.m_v[2];
	return *this;
}


inline Vector_3d &Vector_3d::operator-=(const Vector_3d &v)
{
	m_v[0] -= v.m_v[0];
	m_v[1] -= v.m_v[1];
	m_v[2] -= v.m_v[2];
	return *this;
}


inline Vector_3d &Vector_3d::operator*=(double a)
{
	m_v[0] *= a;
	m_v[1] *= a;
	m_v[2] *= a;
	return *this;
}


inline Vector_3d &Vector_3d::operator*=(const Matrix_4d &m)
{
	double w = m_v[0] * m(3, 0) 
		+ m_v[1] * m(3, 1) 
		+ m_v[2] * m(3, 2) 
		+ m(3, 3);

	double wi = 1.0 / w;

	return reset((m_v[0] * m(0, 0) 
		      + m_v[1] * m(0, 1) 
		      + m_v[2] * m(0, 2) 
		      + m(0, 3)) * wi,

		     (m_v[0] * m(1, 0) 
		      + m_v[1] * m(1, 1)
		      + m_v[2] * m(1, 2) 
		      + m(1, 3)) * wi,

		     (m_v[0] * m(2, 0)
		      + m_v[1] * m(2, 1)
		      + m_v[2] * m(2, 2)
		      + m(2, 3)) * wi);
}


inline Vector_3d &Vector_3d::operator/=(double a)
{
	return *this *= 1.0 / a;
}


inline double Vector_3d::dot(const Vector_3d &v) const
{
	return (m_v[0] * v.m_v[0] +
		m_v[1] * v.m_v[1] +
		m_v[2] * v.m_v[2]);
}


inline double Vector_3d::dot(const Point_3d &p) const
{
	return (m_v[0] * p.m_p[0] +
		m_v[1] * p.m_p[1] +
		m_v[2] * p.m_p[2]);
}


inline double Vector_3d::operator*(const Vector_3d &v) const
{
	return dot(v);
}


inline double Vector_3d::operator*(const Point_3d &p) const
{
	return dot(p);
}


inline Vector_3d operator+(const Vector_3d &u, const Vector_3d &v)
{
	return Vector_3d(u.m_v[0] + v.m_v[0],
			 u.m_v[1] + v.m_v[1],
			 u.m_v[2] + v.m_v[2]);
}


inline Vector_3d operator-(const Vector_3d &u, const Vector_3d &v)
{
	return Vector_3d(u.m_v[0] - v.m_v[0],
			 u.m_v[1] - v.m_v[1],
			 u.m_v[2] - v.m_v[2]);
}


inline Vector_3d operator*(double a, const Vector_3d &v)
{
	return Vector_3d(a * v.m_v[0],
			 a * v.m_v[1],
			 a * v.m_v[2]);
}


inline Vector_3d operator*(const Vector_3d &v, double a)
{
	return a * v;
}


inline Vector_3d operator*(const Matrix_4d &m, const Vector_3d &v)
{
	double w = v.m_v[0] * m(3, 0)
		+ v.m_v[1] * m(3, 1)
		+ v.m_v[2] * m(3, 2)
		+ m(3, 3);

	double wi = 1.0 / w;

	return Vector_3d((v.m_v[0] * m(0, 0)
			  + v.m_v[1] * m(0, 1)
			  + v.m_v[2] * m(0, 2)
			  + m(0, 3)) * wi,

			 (v.m_v[0] * m(1, 0)
			  + v.m_v[1] * m(1, 1)
			  + v.m_v[2] * m(1, 2)
			  + m(1, 3)) * wi,

			 (v.m_v[0] * m(2, 0)
			  + v.m_v[1] * m(2, 1)
			  + v.m_v[2] * m(2, 2)
			  + m(2, 3)) * wi);
}


inline Vector_3d operator/(const Vector_3d &v, double a)
{
	return v * (1.0 / a);
}


inline Vector_3d Vector_3d::cross(const Vector_3d &v) const
{
	return Vector_3d((m_v[1] * v.m_v[2]) - (m_v[2] * v.m_v[1]),
			 (m_v[2] * v.m_v[0]) - (m_v[0] * v.m_v[2]),
			 (m_v[0] * v.m_v[1]) - (m_v[1] * v.m_v[0]));
}


inline double Vector_3d::length() const
{
	return sqrt(this->dot(*this));
}


inline double Vector_3d::squared_length() const
{
	return this->dot(*this);
}


inline bool Vector_3d::eps_is_normalized(double eps) const
{
	return gtb::eps_is_equal(squared_length(), 1.0, eps);
}


inline Vector_3d &Vector_3d::normalize()
{
	*this /= length();
	return *this;
}


inline Vector_3d &Vector_3d::normalize(double &len)
{
	len = length();
	*this /= len;
	return *this;
}


inline Vector_3d &Vector_3d::scale(double a)
{
	*this *= a;
	return *this;
}


inline Vector_3d &Vector_3d::transform(const Matrix_4d &m)
{
	return *this *= m;
}


inline Vector_3d &Vector_3d::affine_transform(const Matrix_4d &m)
{
	return reset(m_v[0] * m(0, 0)
		     + m_v[1] * m(0, 1)
		     + m_v[2] * m(0, 2)
		     + m(0, 3),

		     m_v[0] * m(1, 0)
		     + m_v[1] * m(1, 1)
		     + m_v[2] * m(1, 2)
		     + m(1, 3),

		     m_v[0] * m(2, 0)
		     + m_v[1] * m(2, 1)
		     + m_v[2] * m(2, 2)
		     + m(2, 3));
}


inline void Vector_3d::flip()
{
	m_v[0] = -m_v[0];
	m_v[1] = -m_v[1];
	m_v[2] = -m_v[2];
}


inline void Vector_3d::gl_normal() const
{
	glNormal3dv(m_v);
}


inline void swap(Vector_3d &a, Vector_3d &b)
{
	Vector_3d tmp(a);
	a = b;
	b = tmp;
}


GTB_END_NAMESPACE

#endif // GTB_VECTOR_3D_INCLUDED
