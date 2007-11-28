#ifndef GTB_VECTOR_3F_INCLUDED
#include "Vector_3f.hpp"
#endif

GTB_BEGIN_NAMESPACE


const Vector_3f VECTOR_3F_ZERO(0.0, 0.0, 0.0);
const Vector_3f VECTOR_3F_POSITIVE_X(1.0, 0.0, 0.0);
const Vector_3f VECTOR_3F_NEGATIVE_X(-1.0, 0.0, 0.0);
const Vector_3f VECTOR_3F_POSITIVE_Y(0.0, 1.0, 0.0);
const Vector_3f VECTOR_3F_NEGATIVE_Y(0.0, -1.0, 0.0);
const Vector_3f VECTOR_3F_POSITIVE_Z(0.0, 0.0, 1.0);
const Vector_3f VECTOR_3F_NEGATIVE_Z(0.0, 0.0, -1.0);


Vector_3f &Vector_3f::rotate(const Vector_3f &axis, float theta)
{
	// From Goldstein
	*this =	*this * cos(theta) +
		axis * this->dot(axis) * (1.0 - cos(theta)) -
		this->cross(axis) * sin(theta);
	return *this;
}


// Finds the rotation that takes V to W (Graphics Gems I, p. 531).
// (Here we pre-multiply, though, i.e., M * p, so we have to use the
// transpose of what's shown there.)
Matrix_4d Vector_3f::rotation(const Vector_3f &V, const Vector_3f &W)
{
	float d = V.dot(W);
	if (gtb::eps_is_equal(d, 1.0, FLT_EPSILON)) {
		return MATRIX_4D_IDENTITY;
	}
	if (gtb::eps_is_equal(d, -1.0, FLT_EPSILON)) {
		return -MATRIX_4D_IDENTITY;
	}

	Vector_3f N = V.cross(W);
	N.normalize();

	Vector_3f M = N.cross(V);
	M.normalize();

	Matrix_4d Q(V[0], V[1], V[2], 0.0,
		    M[0], M[1], M[2], 0.0,
		    N[0], N[1], N[2], 0.0,
		    0.0, 0.0, 0.0, 1.0);

	Vector_3f W2 = Q * W;

	Matrix_4d RT(W2[0], -W2[1], 0.0, 0.0,
		     W2[1], W2[0], 0.0, 0.0,
		     W2[2], W2[2], 1.0, 0.0,
		     0.0, 0.0, 0.0, 1.0);

	return Q.transpose() * RT * Q;
}


GTB_END_NAMESPACE
