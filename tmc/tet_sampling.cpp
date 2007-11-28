#include "tet_sampling.hpp"

// This C++ function takes in input four 3D points describing the
// vertexes of a tetrahedron and returns a random point with uniform
// distribution inside it. We assume that the Point3 class has operators
// overloaded with the obvious meaning of product between vectors and scalar,
// sum between vectors, etc.
//
// from:
//    C. Rocchini and P. Cignoni.
//    Generating random points in a tetrahedron.
//    Journal of graphics tools, 5(4):9-12, 2000
//    

Vector_3f Pick(Vector_3f &v0, Vector_3f &v1, Vector_3f &v2, Vector_3f &v3)
{
	double s = drand();
	double t = drand();
	double u = drand();
	if (s+t>1.0) { // cut'n fold the cube into a prism
		s = 1.0 - s;
		t = 1.0 - t;
	}
	if (t+u>1.0) { // cut'n fold the prism into a tetrahedron
		double tmp = u;
		u = 1.0 - s - t;
		t = 1.0 - tmp;
	} else if (s+t+u>1.0) {
		double tmp = u;
		u = s + t + u - 1.0;
		s = 1 - t - tmp;
	}
	double a=1-s-t-u; // a,s,t,u are the barycentric coordinates of the random point.
	return v0*(float)a + v1*(float)s + v2*(float)t + v3*(float)u;
}
