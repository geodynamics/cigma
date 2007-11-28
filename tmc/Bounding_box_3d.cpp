#ifndef GTB_BOUNDING_BOX_3D_INCLUDED
#include "Bounding_box_3d.hpp"
#endif

#ifndef GTB_MATH_UTIL_INCLUDED
#include "math_util.hpp"
#endif

GTB_BEGIN_NAMESPACE


Bounding_box_3d::Relative_position Bounding_box_3d::
relative_position_of(const Bounding_box_3d &b) const
{
	if ((m_max_point[0] < b.m_min_point[0]) ||
	    (m_min_point[0] > b.m_max_point[0]) ||
	    (m_max_point[1] < b.m_min_point[1]) ||
	    (m_min_point[1] > b.m_max_point[1]) ||
	    (m_max_point[2] < b.m_min_point[2]) ||
	    (m_min_point[2] > b.m_max_point[2])) {
		return OUTSIDE;
	}

	if ((m_min_point[0] <= b.m_min_point[0]) &&
	    (m_max_point[0] >= b.m_max_point[0]) &&
	    (m_min_point[1] <= b.m_min_point[1]) &&
	    (m_max_point[1] >= b.m_max_point[1]) &&
	    (m_min_point[2] <= b.m_min_point[2]) &&
	    (m_max_point[2] >= b.m_max_point[2])) {
		return INSIDE;
        }

	return INTERSECTING;
}


// void Bounding_box_3d::render_solid() const
// {
// 	glBegin(GL_QUADS);

// 	// back
// 	glNormal3d(0.0, 0.0, -1.0);
// 	glVertex3d(m_min_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3d(m_min_point[0], m_max_point[1], m_min_point[2]);
// 	glVertex3d(m_max_point[0], m_max_point[1], m_min_point[2]);
// 	glVertex3d(m_max_point[0], m_min_point[1], m_min_point[2]);

// 	// front
// 	glNormal3d(0.0, 0.0, 1.0);
// 	glVertex3d(m_min_point[0], m_min_point[1], m_max_point[2]);
// 	glVertex3d(m_max_point[0], m_min_point[1], m_max_point[2]);
// 	glVertex3d(m_max_point[0], m_max_point[1], m_max_point[2]);
// 	glVertex3d(m_min_point[0], m_max_point[1], m_max_point[2]);

// 	// left
// 	glNormal3d(-1.0, 0.0, 0.0);
// 	glVertex3d(m_min_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3d(m_min_point[0], m_min_point[1], m_max_point[2]);
// 	glVertex3d(m_min_point[0], m_max_point[1], m_max_point[2]);
// 	glVertex3d(m_min_point[0], m_max_point[1], m_min_point[2]);

// 	// right
// 	glNormal3d(1.0, 0.0, 0.0);
// 	glVertex3d(m_max_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3d(m_max_point[0], m_max_point[1], m_min_point[2]);
// 	glVertex3d(m_max_point[0], m_max_point[1], m_max_point[2]);
// 	glVertex3d(m_max_point[0], m_min_point[1], m_max_point[2]);

// 	// top
// 	glNormal3d(0.0, 1.0, 0.0);
// 	glVertex3d(m_min_point[0], m_max_point[1], m_min_point[2]);
// 	glVertex3d(m_min_point[0], m_max_point[1], m_max_point[2]);
// 	glVertex3d(m_max_point[0], m_max_point[1], m_max_point[2]);
// 	glVertex3d(m_max_point[0], m_max_point[1], m_min_point[2]);

// 	// bottom
// 	glNormal3d(0.0, -1.0, 0.0);
// 	glVertex3d(m_min_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3d(m_max_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3d(m_max_point[0], m_min_point[1], m_max_point[2]);
// 	glVertex3d(m_min_point[0], m_min_point[1], m_max_point[2]);

// 	glEnd();
// }


// void Bounding_box_3d::render_solid_no_normals() const
// {
// 	glBegin(GL_QUADS);

// 	// back
// 	glVertex3d(m_min_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3d(m_min_point[0], m_max_point[1], m_min_point[2]);
// 	glVertex3d(m_max_point[0], m_max_point[1], m_min_point[2]);
// 	glVertex3d(m_max_point[0], m_min_point[1], m_min_point[2]);

// 	// front
// 	glVertex3d(m_min_point[0], m_min_point[1], m_max_point[2]);
// 	glVertex3d(m_max_point[0], m_min_point[1], m_max_point[2]);
// 	glVertex3d(m_max_point[0], m_max_point[1], m_max_point[2]);
// 	glVertex3d(m_min_point[0], m_max_point[1], m_max_point[2]);

// 	// left
// 	glVertex3d(m_min_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3d(m_min_point[0], m_min_point[1], m_max_point[2]);
// 	glVertex3d(m_min_point[0], m_max_point[1], m_max_point[2]);
// 	glVertex3d(m_min_point[0], m_max_point[1], m_min_point[2]);

// 	// right
// 	glVertex3d(m_max_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3d(m_max_point[0], m_max_point[1], m_min_point[2]);
// 	glVertex3d(m_max_point[0], m_max_point[1], m_max_point[2]);
// 	glVertex3d(m_max_point[0], m_min_point[1], m_max_point[2]);

// 	// top
// 	glVertex3d(m_min_point[0], m_max_point[1], m_min_point[2]);
// 	glVertex3d(m_min_point[0], m_max_point[1], m_max_point[2]);
// 	glVertex3d(m_max_point[0], m_max_point[1], m_max_point[2]);
// 	glVertex3d(m_max_point[0], m_max_point[1], m_min_point[2]);

// 	// bottom
// 	glVertex3d(m_min_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3d(m_max_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3d(m_max_point[0], m_min_point[1], m_max_point[2]);
// 	glVertex3d(m_min_point[0], m_min_point[1], m_max_point[2]);

// 	glEnd();
// }


// void Bounding_box_3d::outline() const
// {
// 	glPushAttrib(GL_ENABLE_BIT);
// 	glDisable(GL_LIGHTING);

// 	glBegin(GL_LINE_LOOP);
// 	glVertex3f(m_min_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3f(m_max_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3f(m_max_point[0], m_max_point[1], m_min_point[2]);
// 	glVertex3f(m_min_point[0], m_max_point[1], m_min_point[2]);
// 	glEnd();

// 	glBegin(GL_LINE_LOOP);
// 	glVertex3f(m_min_point[0], m_min_point[1], m_max_point[2]);
// 	glVertex3f(m_max_point[0], m_min_point[1], m_max_point[2]);
// 	glVertex3f(m_max_point[0], m_max_point[1], m_max_point[2]);
// 	glVertex3f(m_min_point[0], m_max_point[1], m_max_point[2]);
// 	glEnd();

// 	glBegin(GL_LINES);
// 	glVertex3f(m_min_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3f(m_min_point[0], m_min_point[1], m_max_point[2]);
// 	glVertex3f(m_max_point[0], m_min_point[1], m_min_point[2]);
// 	glVertex3f(m_max_point[0], m_min_point[1], m_max_point[2]);
// 	glVertex3f(m_max_point[0], m_max_point[1], m_min_point[2]);
// 	glVertex3f(m_max_point[0], m_max_point[1], m_max_point[2]);
// 	glVertex3f(m_min_point[0], m_max_point[1], m_min_point[2]);
// 	glVertex3f(m_min_point[0], m_max_point[1], m_max_point[2]);
// 	glEnd();

// 	glPopAttrib();
// }


void Bounding_box_3d::print(FILE *fp) const
{
	const Point_3d &p1 = m_min_point;
	const Point_3d &p2 = m_max_point;
	fprintf(fp, "(%f, %f, %f) (%f %f %f)\n",
		p1[0], p1[1], p1[2], p2[0], p2[1], p2[2]);
}


Point_3d Bounding_box_3d::random_point() const
{
	Vector_3d v((drand() - 0.5) * x_length(),
		    (drand() - 0.5) * y_length(),
		    (drand() - 0.5) * z_length());
	return centroid() + v;
}


double Bounding_box_3d::distance(const Point_3d &p) const
{
	Vector_3d dl = m_min_point - p;
	Vector_3d dr = p - m_max_point;
	for (unsigned i = 0; i < 3; ++i) {
		if (dl[i] < 0) {
			dl[i] = 0;
		}
		if (dr[i] < 0) {
			dr[i] = 0;
		}
	}
	Vector_3d diff = dl + dr;
	double d = diff.length();
	return d;
}


Bounding_box_3d Bounding_box_3d::smallest_cube_containing(
	const Bounding_box_3d &b)
{
	Point_3d c = b.centroid();
	double len = 0.5 * b.longest_axis_length();
	Vector_3d v(1.0, 1.0, 1.0);
	Point_3d p_min = c - len * v;
	Point_3d p_max = c + len * v;
	return Bounding_box_3d(p_min, p_max);
}


// bool Bounding_box_3d::is_visible() const
// {

// /* Copyright (c) Hewlett Packard Company, 1997. */

// /* This program is freely distributable without licensing fees and is
//    provided without guarantee or warrantee expressed or implied. This
//    program is -not- in the public domain. */

// #define GL_OCCLUSION_TEST_HP			0x8165
// #define GL_OCCLUSION_RESULT_HP			0x8166
// #define GL_OCCLUSION_TEST_HP_OLD		0x816E
// #define GL_OCCLUSION_RESULT_HP_OLD		0x816F

// 	GLfloat x1 = m_min_point[0];
// 	GLfloat y1 = m_min_point[1];
// 	GLfloat z1 = m_min_point[2];

// 	GLfloat x2 = m_max_point[0];
// 	GLfloat y2 = m_max_point[1];
// 	GLfloat z2 = m_max_point[2];

// 	GLboolean result = false;

// 	// enable HP test
// //	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
// //	glDisable(GL_LIGHTING);
// 	glDisable(GL_STENCIL_TEST);
// 	glEnable(GL_OCCLUSION_TEST_HP);
// 	glDepthMask(GL_FALSE);
// 	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

// 	// perform HP tests


// 	glBegin(GL_QUADS);

// 	/* Front */
// 	glVertex3f(x1, y1, z1);
// 	glVertex3f(x1, y2, z1);
// 	glVertex3f(x2, y2, z1);
// 	glVertex3f(x2, y1, z1);

// 	/* Right */
// 	glVertex3f(x2, y2, z1);
// 	glVertex3f(x2, y1, z1);
// 	glVertex3f(x2, y1, z2);
// 	glVertex3f(x2, y2, z2);

// 	/* Back */
// 	glVertex3f(x2, y2, z2);
// 	glVertex3f(x2, y1, z2);
// 	glVertex3f(x1, y1, z2);
// 	glVertex3f(x1, y2, z2);

// 	/* Left */
// 	glVertex3f(x1, y2, z2);
// 	glVertex3f(x1, y1, z2);
// 	glVertex3f(x1, y1, z1);
// 	glVertex3f(x1, y2, z1);

// 	/* Top */
// 	glVertex3f(x2, y2, z1);
// 	glVertex3f(x2, y2, z2);
// 	glVertex3f(x1, y2, z2);
// 	glVertex3f(x1, y2, z1);

// 	/* Bottom */
// 	glVertex3f(x2, y1, z1);
// 	glVertex3f(x2, y1, z2);
// 	glVertex3f(x1, y1, z2);
// 	glVertex3f(x1, y1, z1);
// 	glEnd();


// 	// disable HP test
// 	glGetBooleanv(GL_OCCLUSION_TEST_RESULT_HP, &result);
// 	glDisable(GL_OCCLUSION_TEST_HP);
// 	glDepthMask(GL_TRUE);
// 	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
// //	glPopAttrib();
// 	return (result == GL_TRUE);
// }

GTB_END_NAMESPACE
