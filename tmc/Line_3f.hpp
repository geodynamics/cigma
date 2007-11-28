#ifndef GTB_LINE_3F_INCLUDED
#define GTB_LINE_3F_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_POINT_3F_INCLUDED
#include "Point_3f.hpp"
#endif

#ifndef GTB_VECTOR_3F_INCLUDED
#include "Vector_3f.hpp"
#endif

GTB_BEGIN_NAMESPACE


class Line_3f {
public:
	Line_3f();
	Line_3f(const Line_3f &l);
	Line_3f(const Point_3f &p, const Point_3f &q);
	Line_3f(const Point_3f &p, const Vector_3f &d, bool is_normalized);
	Line_3f &operator=(const Line_3f &r);

	bool eps_is_equal(const Line_3f &l, float eps) const;
	bool eps_contains(const Point_3f &p, float eps) const;

	// Returns an arbitrary point on the line.
	// point(i) == point(j), iff i == j.
	Point_3f point(int i) const;

	const Vector_3f &direction() const;

	Point_3f projection(const Point_3f &p) const;

protected:
	Point_3f m_p;
        Vector_3f m_d;
};


inline Line_3f::Line_3f()
{
}


inline Line_3f::Line_3f(const Line_3f &l)
	: m_p(l.m_p),
	  m_d(l.m_d)
{
}


inline Line_3f::Line_3f(const Point_3f &p, const Point_3f &q)
	: m_p(p),
	  m_d(q - p)
{
	m_d.normalize();
}


inline Line_3f::Line_3f(const Point_3f &p,
			const Vector_3f &d,
			bool is_normalized)
	: m_p(p),
	  m_d(d)
{
	if (!is_normalized) {
		m_d.normalize();
	}
}


inline Line_3f &Line_3f::operator=(const Line_3f &l)
{
	m_p = l.m_p;
	m_d = l.m_d;
	return *this;
}


inline bool Line_3f::eps_contains(const Point_3f &p, float eps) const
{
	return (p - m_p).cross(m_d).eps_is_zero(eps);
}


inline bool Line_3f::eps_is_equal(const Line_3f &l, float eps) const
{
	return ((m_d.eps_is_equal(l.m_d, eps) 
		 || m_d.eps_is_equal(-l.m_d, eps))
		&& eps_contains(l.m_p, eps));
}


inline const Vector_3f &Line_3f::direction() const
{
	return m_d;
}


inline Point_3f Line_3f::point(int i) const
{
	return m_p + (i * m_d);
}


inline Point_3f Line_3f::projection(const Point_3f &p) const
{
	return m_p + ((p - m_p).dot(m_d)) * m_d;
}


GTB_END_NAMESPACE

#endif // GTB_LINE_3F_INCLUDED
