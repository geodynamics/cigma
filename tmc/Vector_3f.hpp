#ifndef GTB_VECTOR_3F_INCLUDED
#define GTB_VECTOR_3F_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_GL_HEADERS_INCLUDED
#include "gl_headers.hpp"
#endif

GTB_BEGIN_NAMESPACE

class Point_3f;
class Matrix_4d;


class Vector_3f {
public:
	Vector_3f();
	Vector_3f(const Vector_3f &v);
	explicit Vector_3f(const Point_3f &p);
	Vector_3f(float vx, float vy, float vz);
	explicit Vector_3f(const float v[3]);
	explicit Vector_3f(const float v);
	Vector_3f &operator=(const Vector_3f &v);

	const float &get_x() const;
	const float &get_y() const;
	const float &get_z() const;

	const float &operator[](unsigned i) const;
	float &operator[](unsigned i);

	Vector_3f &set_x(float vx);
	Vector_3f &set_y(float vy);
	Vector_3f &set_z(float vz);
	Vector_3f &reset(float vx, float vy, float vz);

	bool eps_is_equal(const Vector_3f &v, float eps) const;
	bool eps_is_zero(float eps) const;
	bool eps_is_normalized(float eps) const;

	Vector_3f operator-() const;
	Vector_3f &operator*=(const Matrix_4d &m);
	Vector_3f &operator+=(const Vector_3f &v);
	Vector_3f &operator-=(const Vector_3f &v);
	Vector_3f &operator*=(float a);
	Vector_3f &operator/=(float a);

	float operator*(const Vector_3f &v) const;
	float operator*(const Point_3f &p) const;

	friend Vector_3f operator+(const Vector_3f &u, const Vector_3f &v);
	friend Point_3f operator+(const Point_3f &p, const Vector_3f &v);
	friend Vector_3f operator-(const Vector_3f &u, const Vector_3f &v);
	friend Point_3f operator-(const Point_3f &p, const Vector_3f &v);
	friend Vector_3f operator*(float a, const Vector_3f &v);
	friend Vector_3f operator*(const Vector_3f &v, float a);
	friend Vector_3f operator*(const Matrix_4d &m, const Vector_3f &v);
	friend Vector_3f operator/(const Vector_3f &v, float a);

	float dot(const Vector_3f &v) const;
	float dot(const Point_3f &p) const;
	Vector_3f cross(const Vector_3f &v) const;
	float length() const;
	float squared_length() const;
	Vector_3f &normalize();
	Vector_3f &normalize(float &len);
	Vector_3f &scale(float a);
	Vector_3f &rotate(const Vector_3f &axis, float theta);
	Vector_3f &transform(const Matrix_4d &m);
	Vector_3f &affine_transform(const Matrix_4d &m);
	void flip();

	void gl_normal() const;

	static Matrix_4d rotation(const Vector_3f &from, const Vector_3f &to);

	friend class Point_3f;
	friend void swap(Vector_3f &a, Vector_3f &b);

protected:
	float m_v[3];
};


extern const Vector_3f VECTOR_3F_ZERO;
extern const Vector_3f VECTOR_3F_POSITIVE_X;
extern const Vector_3f VECTOR_3F_NEGATIVE_X;
extern const Vector_3f VECTOR_3F_POSITIVE_Y;
extern const Vector_3f VECTOR_3F_NEGATIVE_Y;
extern const Vector_3f VECTOR_3F_POSITIVE_Z;
extern const Vector_3f VECTOR_3F_NEGATIVE_Z;


GTB_END_NAMESPACE


#ifndef GTB_POINT_3F_INCLUDED
#include "Point_3f.hpp"
#endif


GTB_BEGIN_NAMESPACE


inline Vector_3f::Vector_3f()
{
}


inline Vector_3f::Vector_3f(const Vector_3f &v)
{
	m_v[0] = v.m_v[0];
	m_v[1] = v.m_v[1];
	m_v[2] = v.m_v[2];
}


inline Vector_3f::Vector_3f(float vx, float vy, float vz)
{
	m_v[0] = vx;
	m_v[1] = vy;
	m_v[2] = vz;
}


inline Vector_3f::Vector_3f(const float v[3])
{
	m_v[0] = v[0];
	m_v[1] = v[1];
	m_v[2] = v[2];
}


inline Vector_3f::Vector_3f(const float v) 
{
	m_v[0] = v;
	m_v[1] = v;
	m_v[2] = v;
}


inline Vector_3f::Vector_3f(const Point_3f &p)
{
	m_v[0] = p.m_p[0];
	m_v[1] = p.m_p[1];
	m_v[2] = p.m_p[2];
}


inline Vector_3f &Vector_3f::operator=(const Vector_3f &v)
{
	m_v[0] = v.m_v[0];
	m_v[1] = v.m_v[1];
	m_v[2] = v.m_v[2];
	return *this;
}


inline const float &Vector_3f::get_x() const
{
	return m_v[0];
}


inline const float &Vector_3f::get_y() const
{
	return m_v[1];
}


inline const float &Vector_3f::get_z() const
{
	return m_v[2];
}


inline const float &Vector_3f::operator[](unsigned i) const
{
	GTB_REQUIRE(i < 3);
	return m_v[i];
}


inline float &Vector_3f::operator[](unsigned i)
{
	GTB_REQUIRE(i < 3);
	return m_v[i];
}


inline Vector_3f &Vector_3f::set_x(float vx)
{
	m_v[0] = vx;
	return *this;
}


inline Vector_3f &Vector_3f::set_y(float vy)
{
	m_v[1] = vy;
	return *this;
}


inline Vector_3f &Vector_3f::set_z(float vz)
{
	m_v[2] = vz;
	return *this;
}


inline Vector_3f &Vector_3f::reset(float vx, float vy, float vz)
{
	m_v[0] = vx;
	m_v[1] = vy;
	m_v[2] = vz;
	return *this;
}


inline bool Vector_3f::eps_is_equal(const Vector_3f &v, float eps) const
{
	return (gtb::eps_is_equal(m_v[0], v.m_v[0], eps) &&
		gtb::eps_is_equal(m_v[1], v.m_v[1], eps) &&
		gtb::eps_is_equal(m_v[2], v.m_v[2], eps));
}


inline bool Vector_3f::eps_is_zero(float eps) const
{
	return (gtb::eps_is_zero(m_v[0], eps) &&
		gtb::eps_is_zero(m_v[1], eps) &&
		gtb::eps_is_zero(m_v[2], eps));
}


inline Vector_3f Vector_3f::operator-() const
{
	return Vector_3f(-m_v[0],
			 -m_v[1],
			 -m_v[2]);
}


inline Vector_3f &Vector_3f::operator+=(const Vector_3f &v)
{
	m_v[0] += v.m_v[0];
	m_v[1] += v.m_v[1];
	m_v[2] += v.m_v[2];
	return *this;
}


inline Vector_3f &Vector_3f::operator-=(const Vector_3f &v)
{
	m_v[0] -= v.m_v[0];
	m_v[1] -= v.m_v[1];
	m_v[2] -= v.m_v[2];
	return *this;
}


inline Vector_3f &Vector_3f::operator*=(float a)
{
	m_v[0] *= a;
	m_v[1] *= a;
	m_v[2] *= a;
	return *this;
}


inline Vector_3f &Vector_3f::operator*=(const Matrix_4d &m)
{
	float w = m_v[0] * m(3, 0) 
		+ m_v[1] * m(3, 1) 
		+ m_v[2] * m(3, 2) 
		+ m(3, 3);

	float wi = 1.0 / w;

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


inline Vector_3f &Vector_3f::operator/=(float a)
{
	return *this *= 1.0 / a;
}


inline float Vector_3f::dot(const Vector_3f &v) const
{
	return (m_v[0] * v.m_v[0] +
		m_v[1] * v.m_v[1] +
		m_v[2] * v.m_v[2]);
}


inline float Vector_3f::dot(const Point_3f &p) const
{
	return (m_v[0] * p.m_p[0] +
		m_v[1] * p.m_p[1] +
		m_v[2] * p.m_p[2]);
}


inline float Vector_3f::operator*(const Vector_3f &v) const
{
	return dot(v);
}


inline float Vector_3f::operator*(const Point_3f &p) const
{
	return dot(p);
}


inline Vector_3f operator+(const Vector_3f &u, const Vector_3f &v)
{
	return Vector_3f(u.m_v[0] + v.m_v[0],
			 u.m_v[1] + v.m_v[1],
			 u.m_v[2] + v.m_v[2]);
}


inline Vector_3f operator-(const Vector_3f &u, const Vector_3f &v)
{
	return Vector_3f(u.m_v[0] - v.m_v[0],
			 u.m_v[1] - v.m_v[1],
			 u.m_v[2] - v.m_v[2]);
}


inline Vector_3f operator*(float a, const Vector_3f &v)
{
	return Vector_3f(a * v.m_v[0],
			 a * v.m_v[1],
			 a * v.m_v[2]);
}


inline Vector_3f operator*(const Vector_3f &v, float a)
{
	return a * v;
}


inline Vector_3f operator*(const Matrix_4d &m, const Vector_3f &v)
{
	float w = v.m_v[0] * m(3, 0)
		+ v.m_v[1] * m(3, 1)
		+ v.m_v[2] * m(3, 2)
		+ m(3, 3);

	float wi = 1.0 / w;

	return Vector_3f((v.m_v[0] * m(0, 0)
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


inline Vector_3f operator/(const Vector_3f &v, float a)
{
	return v * (1.0 / a);
}


inline Vector_3f Vector_3f::cross(const Vector_3f &v) const
{
	return Vector_3f((m_v[1] * v.m_v[2]) - (m_v[2] * v.m_v[1]),
			 (m_v[2] * v.m_v[0]) - (m_v[0] * v.m_v[2]),
			 (m_v[0] * v.m_v[1]) - (m_v[1] * v.m_v[0]));
}


inline float Vector_3f::length() const
{
	return sqrt(this->dot(*this));
}


inline float Vector_3f::squared_length() const
{
	return this->dot(*this);
}


inline bool Vector_3f::eps_is_normalized(float eps) const
{
	return gtb::eps_is_equal(squared_length(), 1.0, eps);
}


inline Vector_3f &Vector_3f::normalize()
{
	*this /= length();
	return *this;
}


inline Vector_3f &Vector_3f::normalize(float &len)
{
	len = length();
	*this /= len;
	return *this;
}


inline Vector_3f &Vector_3f::scale(float a)
{
	*this *= a;
	return *this;
}


inline Vector_3f &Vector_3f::transform(const Matrix_4d &m)
{
	return *this *= m;
}


inline Vector_3f &Vector_3f::affine_transform(const Matrix_4d &m)
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


inline void Vector_3f::flip()
{
	m_v[0] = -m_v[0];
	m_v[1] = -m_v[1];
	m_v[2] = -m_v[2];
}


inline void Vector_3f::gl_normal() const
{
	glNormal3fv(m_v);
}


inline void swap(Vector_3f &a, Vector_3f &b)
{
	Vector_3f tmp(a);
	a = b;
	b = tmp;
}


GTB_END_NAMESPACE

#endif // GTB_VECTOR_3F_INCLUDED
