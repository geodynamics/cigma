#ifndef GTB_VECTOR_3B_INCLUDED
#define GTB_VECTOR_3B_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_ASSERTIONS_INCLUDED
#include "assertions.hpp"
#endif

#ifndef GTB_GL_HEADERS_INCLUDED
#include "gl_headers.hpp"
#endif

GTB_BEGIN_NAMESPACE


class Vector_3b {
public:
	Vector_3b();
	Vector_3b(GLbyte x, GLbyte y, GLbyte z);
	explicit Vector_3b(GLbyte v[3]);
	//Vector_3b(const Vector_3b &v);
	//Vector_3b &operator=(const Vector_3b &v);

	const GLbyte &get_x() const;
	const GLbyte &get_y() const;
	const GLbyte &get_z() const;

	const GLbyte &operator[](unsigned i) const;
	GLbyte &operator[](unsigned i);

	Vector_3b &set_x(GLbyte x);
	Vector_3b &set_y(GLbyte y);
	Vector_3b &set_z(GLbyte z);
	Vector_3b &reset(GLbyte x, GLbyte y, GLbyte z);

	void flip();

	void gl_normal() const;

protected:
	GLbyte m_v[3];
};


inline Vector_3b::Vector_3b()
{
}


inline Vector_3b::Vector_3b(GLbyte x, GLbyte y, GLbyte z)
{
	m_v[0] = x;
	m_v[1] = y;
	m_v[2] = z;
}


inline Vector_3b::Vector_3b(GLbyte v[3])
{
	m_v[0] = v[0];
	m_v[1] = v[1];
	m_v[2] = v[2];
}


// inline Vector_3b::Vector_3b(const Vector_3b &v)
// {
// 	m_v[0] = v.m_v[0];
// 	m_v[1] = v.m_v[1];
// 	m_v[2] = v.m_v[2];
// }


// inline Vector_3b &Vector_3b::operator=(const Vector_3b &v)
// {
// 	m_v[0] = v.m_v[0];
// 	m_v[1] = v.m_v[1];
// 	m_v[2] = v.m_v[2];
// 	return *this;
// }


inline const GLbyte &Vector_3b::get_x() const
{
	return m_v[0];
}


inline const GLbyte &Vector_3b::get_y() const
{
	return m_v[1];
}


inline const GLbyte &Vector_3b::get_z() const
{
	return m_v[2];
}


inline const GLbyte &Vector_3b::operator[](unsigned i) const
{
	GTB_REQUIRE(i < 3);
	return m_v[i];
}


inline GLbyte &Vector_3b::operator[](unsigned i)
{
	GTB_REQUIRE(i < 3);
	return m_v[i];
}


inline Vector_3b &Vector_3b::set_x(GLbyte x)
{
	m_v[0] = x;
	return *this;
}


inline Vector_3b &Vector_3b::set_y(GLbyte y)
{
	m_v[1] = y;
	return *this;
}


inline Vector_3b &Vector_3b::set_z(GLbyte z)
{
	m_v[2] = z;
	return *this;
}


inline Vector_3b &Vector_3b::reset(GLbyte x, GLbyte y, GLbyte z)
{
	m_v[0] = x;
	m_v[1] = y;
	m_v[2] = z;
	return *this;
}


inline void Vector_3b::flip()
{
	m_v[0] = -m_v[0];
	m_v[1] = -m_v[1];
	m_v[2] = -m_v[2];
}


inline void Vector_3b::gl_normal() const
{
	glNormal3bv(m_v);
}


GTB_END_NAMESPACE

#endif // GTB_VECTOR_3B_INCLUDED
