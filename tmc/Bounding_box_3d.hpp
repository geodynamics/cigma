#ifndef GTB_BOUNDING_BOX_3D_INCLUDED
#define GTB_BOUNDING_BOX_3D_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_POINT_3F_INCLUDED
#include "Point_3f.hpp"
#endif

#ifndef GTB_POINT_3D_INCLUDED
#include "Point_3d.hpp"
#endif

#ifndef GTB_C_STDIO_INCLUDED
#include <stdio.h>
#define GTB_C_STDIO_INCLUDED
#endif

GTB_BEGIN_NAMESPACE


class Bounding_box_3d {
public:
	// initialize to empty box
	Bounding_box_3d();

	// initialize explicitly
	Bounding_box_3d(double min_x, double min_y, double min_z,
			double max_x, double max_y, double max_z);

	// create given 2 points
	Bounding_box_3d(const Point_3d &a, const Point_3d &b);
	Bounding_box_3d(const Point_3f &a, const Point_3f &b);

	// create given 3 points
	Bounding_box_3d(const Point_3d &a,
			const Point_3d &b,
			const Point_3d &c);

	Bounding_box_3d(const Point_3f &a,
			const Point_3f &b,
			const Point_3f &c);

	// create given 4 points
	Bounding_box_3d(const Point_3d &a,
			const Point_3d &b,
			const Point_3d &c,
			const Point_3d &d);

	Bounding_box_3d(const Point_3f &a,
			const Point_3f &b,
			const Point_3f &c,
			const Point_3f &d);

	// create given C array of points
	Bounding_box_3d(const Point_3d v[], unsigned point_count);
	Bounding_box_3d(const Point_3f v[], unsigned point_count);

	// create given std::vector of points
	Bounding_box_3d(const std::vector<Point_3d> &v);
	Bounding_box_3d(const std::vector<Point_3f> &v);

	// create given centroid and axes lengths
	Bounding_box_3d(const Point_3d &centroid,
			double x_length,
			double y_length, 
			double z_length);

	// make union
	Bounding_box_3d(const Bounding_box_3d &b1, const Bounding_box_3d &b2);

	// copy
	Bounding_box_3d(const Bounding_box_3d &b);
	Bounding_box_3d &operator=(const Bounding_box_3d &b);

	// query
	bool eps_is_equal(const Bounding_box_3d &b, double eps) const;
	bool eps_is_empty(double eps) const;

	// get
	const Point_3d &get_min_point() const;
	const Point_3d &get_max_point() const;
	double get_min_x() const;
	double get_min_y() const;
	double get_min_z() const;
	double get_max_x() const;
	double get_max_y() const;
	double get_max_z() const;

	// set
	Bounding_box_3d &set_min_point(const Point_3d &min_point);
	Bounding_box_3d &set_max_point(const Point_3d &max_point);
	Bounding_box_3d &reset(const Point_3d &min_point,
			       const Point_3d &max_point);
	Bounding_box_3d &reset(double min_x, double min_y, double min_z,
			       double max_x, double max_y, double max_z);
	Bounding_box_3d &set_min_x(double min_x);
	Bounding_box_3d &set_min_y(double min_y);
	Bounding_box_3d &set_min_z(double min_z);
	Bounding_box_3d &set_max_x(double max_x);
	Bounding_box_3d &set_max_y(double max_y);
	Bounding_box_3d &set_max_z(double max_z);

	void set_min(int axis, double value);
	void set_max(int axis, double value);
	void scale(double s);

	// compute
	Point_3d centroid() const;
	double x_length() const;
	double y_length() const;
	double z_length() const;
	double diagonal_squared_length() const;
	double diagonal_length() const;
	double shortest_axis_length() const;
	double longest_axis_length() const;
	double volume() const;
	bool contains(const Point_3d &p) const;
	bool contains(const Point_3f &p) const;
	Point_3d random_point() const;
	double distance(const Point_3d &p) const;
	Bounding_box_3d smallest_cube_containing(const Bounding_box_3d &b);
	bool is_visible() const;

	enum Relative_position { INSIDE, INTERSECTING, OUTSIDE };
	Relative_position relative_position_of(const Bounding_box_3d &b) const;

	// render
	void render_solid() const;
	void render_solid_no_normals() const;
	void outline() const;

	void print(FILE *fp) const;

protected:
	bool is_order_correct(double eps) const;

	Point_3d m_min_point;
	Point_3d m_max_point;
};


inline bool Bounding_box_3d::is_order_correct(double eps) const
{
	return (gtb::eps_is_less_or_equal(m_min_point[0],
					  m_max_point[0],
					  eps)
		&& gtb::eps_is_less_or_equal(m_min_point[1],
					     m_max_point[1],
					     eps)
		&& gtb::eps_is_less_or_equal(m_min_point[2],
					     m_max_point[2],
					     eps));
}


inline Bounding_box_3d::Bounding_box_3d()
	: m_min_point(FLT_MAX, FLT_MAX, FLT_MAX),
	  m_max_point(-FLT_MAX, -FLT_MAX, -FLT_MAX)
{
	GTB_CHECK(eps_is_empty(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(
	double min_x, double min_y, double min_z,
	double max_x, double max_y, double max_z)
	: m_min_point(min_x, min_y, min_z),
	  m_max_point(max_x, max_y, max_z)
{
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const Point_3d &a, const Point_3d &b)
	: m_min_point(MIN(a[0], b[0]),
		      MIN(a[1], b[1]),
		      MIN(a[2], b[2])),

	  m_max_point(MAX(a[0], b[0]),
		      MAX(a[1], b[1]),
		      MAX(a[2], b[2]))
{
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const Point_3f &a, const Point_3f &b)
	: m_min_point(MIN(a[0], b[0]),
		      MIN(a[1], b[1]),
		      MIN(a[2], b[2])),

	  m_max_point(MAX(a[0], b[0]),
		      MAX(a[1], b[1]),
		      MAX(a[2], b[2]))
{
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const Point_3d &a,
					const Point_3d &b,
					const Point_3d &c)
	: m_min_point(MIN(MIN(a[0], b[0]), c[0]),
		      MIN(MIN(a[1], b[1]), c[1]),
		      MIN(MIN(a[2], b[2]), c[2])),

	  m_max_point(MAX(MAX(a[0], b[0]), c[0]),
		      MAX(MAX(a[1], b[1]), c[1]),
		      MAX(MAX(a[2], b[2]), c[2]))
{
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const Point_3f &a,
					const Point_3f &b,
					const Point_3f &c)
	: m_min_point(MIN(MIN(a[0], b[0]), c[0]),
		      MIN(MIN(a[1], b[1]), c[1]),
		      MIN(MIN(a[2], b[2]), c[2])),

	  m_max_point(MAX(MAX(a[0], b[0]), c[0]),
		      MAX(MAX(a[1], b[1]), c[1]),
		      MAX(MAX(a[2], b[2]), c[2]))
{
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const Point_3d &a,
					const Point_3d &b,
					const Point_3d &c,
					const Point_3d &d)
	: m_min_point(MIN(MIN(MIN(a[0], b[0]), c[0]), d[0]),
		      MIN(MIN(MIN(a[1], b[1]), c[1]), d[1]),
		      MIN(MIN(MIN(a[2], b[2]), c[2]), d[2])),

	  m_max_point(MAX(MAX(MAX(a[0], b[0]), c[0]), d[0]),
		      MAX(MAX(MAX(a[1], b[1]), c[1]), d[1]),
		      MAX(MAX(MAX(a[2], b[2]), c[2]), d[2]))
{
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const Point_3f &a,
					const Point_3f &b,
					const Point_3f &c,
					const Point_3f &d)
	: m_min_point(MIN(MIN(MIN(a[0], b[0]), c[0]), d[0]),
		      MIN(MIN(MIN(a[1], b[1]), c[1]), d[1]),
		      MIN(MIN(MIN(a[2], b[2]), c[2]), d[2])),

	  m_max_point(MAX(MAX(MAX(a[0], b[0]), c[0]), d[0]),
		      MAX(MAX(MAX(a[1], b[1]), c[1]), d[1]),
		      MAX(MAX(MAX(a[2], b[2]), c[2]), d[2]))
{
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const Point_3d v[],
					unsigned point_count)
{
	GTB_REQUIRE(point_count > 0);
	m_min_point = v[0];
	m_max_point = v[0];
	for (unsigned i = 1; i < point_count; i++) {
		const Point_3d &p = v[i];

		m_min_point.set_x(MIN(m_min_point[0], p[0]));
		m_min_point.set_y(MIN(m_min_point[1], p[1]));
		m_min_point.set_z(MIN(m_min_point[2], p[2]));

		m_max_point.set_x(MAX(m_max_point[0], p[0]));
		m_max_point.set_y(MAX(m_max_point[1], p[1]));
		m_max_point.set_z(MAX(m_max_point[2], p[2]));
	}
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const Point_3f v[],
					unsigned point_count)
{
	GTB_REQUIRE(point_count > 0);
	m_min_point.reset(v[0][0], v[0][1], v[0][2]);
	m_max_point.reset(v[0][0], v[0][1], v[0][2]);
	for (unsigned i = 1; i < point_count; i++) {
		const Point_3f &p = v[i];

		m_min_point.set_x(MIN(m_min_point[0], (double) p[0]));
		m_min_point.set_y(MIN(m_min_point[1], (double) p[1]));
		m_min_point.set_z(MIN(m_min_point[2], (double) p[2]));

		m_max_point.set_x(MAX(m_max_point[0], (double) p[0]));
		m_max_point.set_y(MAX(m_max_point[1], (double) p[1]));
		m_max_point.set_z(MAX(m_max_point[2], (double) p[2]));
	}
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const std::vector<Point_3d> &v)
{
	GTB_REQUIRE(v.size() > 0);
	m_min_point = v[0];
	m_max_point = v[0];
	for (unsigned i = 1; i < v.size(); i++) {
		const Point_3d &p = v[i];

		m_min_point.set_x(MIN(m_min_point[0], p[0]));
		m_min_point.set_y(MIN(m_min_point[1], p[1]));
		m_min_point.set_z(MIN(m_min_point[2], p[2]));

		m_max_point.set_x(MAX(m_max_point[0], p[0]));
		m_max_point.set_y(MAX(m_max_point[1], p[1]));
		m_max_point.set_z(MAX(m_max_point[2], p[2]));
	}
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const std::vector<Point_3f> &v)
{
	GTB_REQUIRE(v.size() > 0);
	m_min_point.reset(v[0][0], v[0][1], v[0][2]);
	m_max_point.reset(v[0][0], v[0][1], v[0][2]);
	for (unsigned i = 1; i < v.size(); i++) {
		const Point_3f &p = v[i];

		m_min_point.set_x(MIN(m_min_point[0], (double) p[0]));
		m_min_point.set_y(MIN(m_min_point[1], (double) p[1]));
		m_min_point.set_z(MIN(m_min_point[2], (double) p[2]));

		m_max_point.set_x(MAX(m_max_point[0], (double) p[0]));
		m_max_point.set_y(MAX(m_max_point[1], (double) p[1]));
		m_max_point.set_z(MAX(m_max_point[2], (double) p[2]));
	}
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const Point_3d &c,
					double lx,
					double ly, 
					double lz)
	: m_min_point(c[0] - lx / 2.0,
		      c[1] - ly / 2.0,
		      c[2] - lz / 2.0),

	  m_max_point(c[0] + lx / 2.0,
		      c[1] + ly / 2.0,
		      c[2] + lz / 2.0)
{
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const Bounding_box_3d &b1,
					const Bounding_box_3d &b2)
	: m_min_point(MIN(b1.get_min_x(), b2.get_min_x()),
		      MIN(b1.get_min_y(), b2.get_min_y()),
		      MIN(b1.get_min_z(), b2.get_min_z())),

	  m_max_point(MAX(b1.get_max_x(), b2.get_max_x()),
		      MAX(b1.get_max_y(), b2.get_max_y()),
		      MAX(b1.get_max_z(), b2.get_max_z()))
{
	GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d::Bounding_box_3d(const Bounding_box_3d &b)
	: m_min_point(b.m_min_point),
	  m_max_point(b.m_max_point)
{
	// OK to be incorrect
	// GTB_CHECK(is_order_correct(FLT_EPSILON));
}


inline Bounding_box_3d &Bounding_box_3d::operator=(const Bounding_box_3d &b)
{
	m_min_point = b.m_min_point;
	m_max_point = b.m_max_point;
	return *this;
}


inline bool Bounding_box_3d::eps_is_equal(const Bounding_box_3d &b,
					  double eps) const
{
	return (m_min_point.eps_is_equal(b.m_min_point, eps) &&
		m_max_point.eps_is_equal(b.m_max_point, eps));
}


inline bool Bounding_box_3d::eps_is_empty(double eps) const
{
	return !is_order_correct(eps);
}


inline const Point_3d &Bounding_box_3d::get_min_point() const
{
	return m_min_point;
}


inline const Point_3d &Bounding_box_3d::get_max_point() const
{
	return m_max_point;
}


inline double Bounding_box_3d::get_min_x() const
{
	return m_min_point[0];
}


inline double Bounding_box_3d::get_min_y() const
{
	return m_min_point[1];
}


inline double Bounding_box_3d::get_min_z() const
{
	return m_min_point[2];
}


inline double Bounding_box_3d::get_max_x() const
{
	return m_max_point[0];
}


inline double Bounding_box_3d::get_max_y() const
{
	return m_max_point[1];
}


inline double Bounding_box_3d::get_max_z() const
{
	return m_max_point[2];
}


inline Bounding_box_3d &Bounding_box_3d::set_min_point(
	const Point_3d &min_point)
{
	m_min_point = min_point;
	GTB_CHECK(is_order_correct(FLT_EPSILON));
	return *this;
}


inline Bounding_box_3d &Bounding_box_3d::set_max_point(
	const Point_3d &max_point)
{
	m_max_point = max_point;
	GTB_CHECK(is_order_correct(FLT_EPSILON));
	return *this;
}


inline Bounding_box_3d &Bounding_box_3d::reset(const Point_3d &min_point,
					       const Point_3d &max_point)
{
	m_min_point = min_point;
	m_max_point = max_point;
	GTB_CHECK(is_order_correct(FLT_EPSILON));
	return *this;
}


inline Bounding_box_3d &Bounding_box_3d::reset(
	double min_x, double min_y, double min_z,
	double max_x, double max_y, double max_z)
{
	m_min_point.reset(min_x, min_y, min_z);
	m_max_point.reset(max_x, max_y, max_z);
	GTB_CHECK(is_order_correct(FLT_EPSILON));
	return *this;
}


inline Bounding_box_3d &Bounding_box_3d::set_min_x(double min_x)
{
	m_min_point.set_x(min_x);
	GTB_CHECK(is_order_correct(FLT_EPSILON));
	return *this;
}


inline Bounding_box_3d &Bounding_box_3d::set_min_y(double min_y)
{
	m_min_point.set_y(min_y);
	GTB_CHECK(is_order_correct(FLT_EPSILON));
	return *this;
}


inline Bounding_box_3d &Bounding_box_3d::set_min_z(double min_z)
{
	m_min_point.set_z(min_z);
	GTB_CHECK(is_order_correct(FLT_EPSILON));
	return *this;
}


inline Bounding_box_3d &Bounding_box_3d::set_max_x(double max_x)
{
	m_max_point.set_x(max_x);
	GTB_CHECK(is_order_correct(FLT_EPSILON));
	return *this;
}


inline Bounding_box_3d &Bounding_box_3d::set_max_y(double max_y)
{
	m_max_point.set_y(max_y);
	GTB_CHECK(is_order_correct(FLT_EPSILON));
	return *this;
}


inline Bounding_box_3d &Bounding_box_3d::set_max_z(double max_z)
{
	m_max_point.set_z(max_z);
	GTB_CHECK(is_order_correct(FLT_EPSILON));
	return *this;
}


inline Point_3d Bounding_box_3d::centroid() const
{
	return Point_3d((m_min_point[0] + m_max_point[0]) / 2.0,
			(m_min_point[1] + m_max_point[1]) / 2.0,
			(m_min_point[2] + m_max_point[2]) / 2.0);
}


inline double Bounding_box_3d::x_length() const
{
	return m_max_point[0] - m_min_point[0];
}


inline double Bounding_box_3d::y_length() const
{
	return m_max_point[1] - m_min_point[1];
}


inline double Bounding_box_3d::z_length() const
{
	return m_max_point[2] - m_min_point[2];
}


inline double Bounding_box_3d::diagonal_squared_length() const
{
	return (m_max_point - m_min_point).squared_length();
}


inline double Bounding_box_3d::diagonal_length() const
{
	return sqrt(diagonal_squared_length());
}


inline double Bounding_box_3d::shortest_axis_length() const
{
	double dx = x_length();
	double dy = y_length();
	double dz = z_length();
	return MIN(MIN(dx, dy), dz);
}


inline double Bounding_box_3d::longest_axis_length() const
{
	double dx = x_length();
	double dy = y_length();
	double dz = z_length();
	return MAX(MAX(dx, dy), dz);
}


inline double Bounding_box_3d::volume() const
{
	return x_length() * y_length() * z_length();
}


inline bool Bounding_box_3d::contains(const Point_3d &p) const
{
	return ((p[0] >= m_min_point[0]) && (p[0] <= m_max_point[0]) &&
		(p[1] >= m_min_point[1]) && (p[1] <= m_max_point[1]) &&
		(p[2] >= m_min_point[2]) && (p[2] <= m_max_point[2]));
}


inline bool Bounding_box_3d::contains(const Point_3f &p) const
{
	return ((p[0] >= m_min_point[0]) && (p[0] <= m_max_point[0]) &&
		(p[1] >= m_min_point[1]) && (p[1] <= m_max_point[1]) &&
		(p[2] >= m_min_point[2]) && (p[2] <= m_max_point[2]));
}


inline void Bounding_box_3d::set_min(int axis, double value)
{
	GTB_REQUIRE((axis >= 0) && (axis < 3));
	m_min_point[axis] = value;
}


inline void Bounding_box_3d::set_max(int axis, double value)
{
	GTB_REQUIRE((axis >= 0) && (axis < 3));
	m_max_point[axis] = value;
}


inline void Bounding_box_3d::scale(double s)
{
	Point_3d origin = centroid();
	m_min_point.scale(origin, s);
	m_max_point.scale(origin, s);
}


GTB_END_NAMESPACE

#endif // GTB_BOUNDING_BOX_3D_INCLUDED
