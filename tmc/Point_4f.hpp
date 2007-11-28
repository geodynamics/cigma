#ifndef GTB_POINT_4F_INCLUDED
#define GTB_POINT_4F_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_EPSILON_INCLUDED
#include "epsilon.hpp"
#endif

#ifndef GTB_MATRIX_4d_INCLUDED
#include "Matrix_4d.hpp"
#endif

#ifndef GTB_GL_HEADERS_INCLUDED
#include "gl_headers.hpp"
#endif

GTB_BEGIN_NAMESPACE


class Point_4f {
public:
	Point_4f();
	Point_4f(const Point_4f &p);
	Point_4f(float px, float py, float pz, float pw);
	Point_4f(const float p[4]);
	Point_4f &operator=(const Point_4f &p);

	const float &get_x() const;
	const float &get_y() const;
	const float &get_z() const;
	const float &get_w() const;
	void get(float p[4]) const;
	void get(float &px, float &py, float &pz, float &pw);

	const float &operator[](unsigned i) const;
	float &operator[](unsigned i);

	Point_4f &set_x(float px);
	Point_4f &set_y(float py);
	Point_4f &set_z(float pz);
	Point_4f &set_w(float pw);
	Point_4f &set(float px, float py, float pz, float pw);
	Point_4f &set(const float p[4]);

	bool eps_is_equal(const Point_4f &p, float eps) const;
	bool eps_is_zero(float eps) const;

protected:
	float m_p[4];
};


extern const Point_4f POINT_4F_ZERO;


inline Point_4f::Point_4f()
{
}


inline Point_4f::Point_4f(const Point_4f &p)
{
	m_p[0] = p.m_p[0];
	m_p[1] = p.m_p[1];
	m_p[2] = p.m_p[2];
	m_p[3] = p.m_p[3];
}


inline Point_4f::Point_4f(float px, float py, float pz, float pw)
{
	m_p[0] = px;
	m_p[1] = py;
	m_p[2] = pz;
	m_p[3] = pw;
}


inline Point_4f::Point_4f(const float p[4])
{
	m_p[0] = p[0];
	m_p[1] = p[1];
	m_p[2] = p[2];
	m_p[3] = p[3];
}


inline Point_4f &Point_4f::operator=(const Point_4f &p)
{
	m_p[0] = p.m_p[0];
	m_p[1] = p.m_p[1];
	m_p[2] = p.m_p[2];
	m_p[3] = p.m_p[3];
	return *this;
}


inline bool Point_4f::eps_is_equal(const Point_4f &p, float eps) const
{
	return (gtb::eps_is_equal(m_p[0], p.m_p[0], eps) &&
		gtb::eps_is_equal(m_p[1], p.m_p[1], eps) &&
		gtb::eps_is_equal(m_p[2], p.m_p[2], eps) &&
		gtb::eps_is_equal(m_p[3], p.m_p[3], eps));
}


inline bool Point_4f::eps_is_zero(float eps) const
{
	return (gtb::eps_is_zero(m_p[0], eps) &&
		gtb::eps_is_zero(m_p[1], eps) &&
		gtb::eps_is_zero(m_p[2], eps) &&
		gtb::eps_is_zero(m_p[3], eps));
}


inline const float &Point_4f::get_x() const
{
	return m_p[0];
}


inline const float &Point_4f::get_y() const
{
	return m_p[1];
}


inline const float &Point_4f::get_z() const
{
	return m_p[2];
}


inline const float &Point_4f::get_w() const
{
	return m_p[3];
}


inline void Point_4f::get(float &px, float &py, float &pz, float &pw)
{
	px = m_p[0];
	py = m_p[1];
	pz = m_p[2];
	pw = m_p[3];
}


inline void Point_4f::get(float p[4]) const
{
	p[0] = m_p[0];
	p[1] = m_p[1];
	p[2] = m_p[2];
	p[3] = m_p[3];
}


inline const float &Point_4f::operator[](unsigned i) const
{
	GTB_REQUIRE(i < 4);
	return m_p[i];
}


inline float &Point_4f::operator[](unsigned i)
{
	GTB_REQUIRE(i < 4);
	return m_p[i];
}


inline Point_4f &Point_4f::set_x(float px)
{
	m_p[0] = px;
	return *this;
}


inline Point_4f &Point_4f::set_y(float py)
{
	m_p[1] = py;
	return *this;
}


inline Point_4f &Point_4f::set_z(float pz)
{
	m_p[2] = pz;
	return *this;
}


inline Point_4f &Point_4f::set_w(float pw)
{
	m_p[3] = pw;
	return *this;
}


inline Point_4f &Point_4f::set(float px, float py, float pz, float pw)
{
	m_p[0] = px;
	m_p[1] = py;
	m_p[2] = pz;
	m_p[3] = pw;
	return *this;
}


inline Point_4f &Point_4f::set(const float p[4])
{
	m_p[0] = p[0];
	m_p[1] = p[1];
	m_p[2] = p[2];
	m_p[3] = p[3];
	return *this;
}


GTB_END_NAMESPACE

#endif // GTB_POINT_4F_INCLUDED
