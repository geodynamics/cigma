#ifndef GTB_PLANE_3F_INCLUDED
#define GTB_PLANE_3F_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_VECTOR_3F_INCLUDED
#include "Vector_3f.hpp"
#endif

#ifndef GTB_PLANE_RST_3D_INCLUDED
#include "Plane_RST_3d.hpp"
#endif

#ifndef GTB_MATH_INCLUDED
#include "math.hpp"
#endif

GTB_BEGIN_NAMESPACE


class Plane_3f {
public:
	Plane_3f();
	Plane_3f(const Plane_3f &p);
	Plane_3f(const Vector_3f &normal, float D);
	Plane_3f(const Point_3f &A, const Point_3f &B, const Point_3f &C);
	Plane_3f(const Plane_RST_3d &rhs);
	Plane_3f &operator=(const Plane_3f &p);

	const Vector_3f &get_normal() const;
	const float &get_D() const;

	void set_normal(const Vector_3f &n);
	void set_D(float D);

	float distance(const Point_3f &p) const;
	float signed_distance(const Point_3f &p) const;

	void smallest_normal_axes(unsigned &axis1, unsigned &axis2) const;
	void flip();

	bool eps_contains(const Point_3f &p, float eps) const;

protected:
	// n.p + D = 0
	Vector_3f m_n;
	float m_D;
};


inline Plane_3f::Plane_3f()
{
}


inline Plane_3f::Plane_3f(const Plane_3f &p)
	: m_n(p.m_n),
	  m_D(p.m_D)
{
}


inline Plane_3f::Plane_3f(const Vector_3f &n, float D)
	: m_n(n),
	  m_D(D)
{
}


inline Plane_3f::Plane_3f(const Point_3f &A,
			  const Point_3f &B,
			  const Point_3f &C)
	: m_n(Point_3f::normal(A, B, C)),
	  m_D(-(m_n * A))
{
}


// D = sin(rhs.t) because that's the way the optimization function
// computes things, the idea is to find a minimum of d, we do not want
// d=infinity because of numerical problems.
inline Plane_3f::Plane_3f(const Plane_RST_3d &p) :
	m_n(cos(p.get_r()) * cos(p.get_s()),
	    sin(p.get_r()) * cos(p.get_s()),
	    sin(p.get_s())),
	m_D(p.get_t())		// WAS sin(p.get_t())
{
}


inline Plane_3f &Plane_3f::operator=(const Plane_3f &p)
{
	m_n = p.m_n;
	m_D = p.m_D;
	return *this;
}


inline const Vector_3f &Plane_3f::get_normal() const
{
	return m_n;
}


inline const float &Plane_3f::get_D() const
{
	return m_D;
}


inline void Plane_3f::set_normal(const Vector_3f &n)
{
	m_n = n;
}


inline void Plane_3f::set_D(float D)
{
	m_D = D;
}


inline float Plane_3f::signed_distance(const Point_3f &p) const
{
	return (m_n * p + m_D);
}


inline float Plane_3f::distance(const Point_3f &p) const
{
	return abs(signed_distance(p));
}


inline void Plane_3f::flip()
{
	m_n.flip();
	m_D = -m_D;
}


inline void Plane_3f::smallest_normal_axes(unsigned &axis1,
					   unsigned &axis2) const
{
	if ((fabs(m_n[0]) > fabs(m_n[1])) &&
	    (fabs(m_n[0]) > fabs(m_n[2]))) {
		axis1 = 1;
		axis2 = 2;
	} else if (fabs(m_n[1]) > fabs(m_n[2])) {
		axis1 = 0;
		axis2 = 2;
	} else {
		axis1 = 0;
		axis2 = 1;
	}
}


inline bool Plane_3f::eps_contains(const Point_3f &p, float eps) const
{
	return gtb::eps_is_zero(signed_distance(p), eps);
}


GTB_END_NAMESPACE

#endif // GTB_PLANE_3F_INCLUDED
