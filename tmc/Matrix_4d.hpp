#ifndef GTB_MATRIX_4D_INCLUDED
#define GTB_MATRIX_4D_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_ASSERTIONS_INCLUDED
#include "assertions.hpp"
#endif

#ifndef GTB_EPSILON_INCLUDED
#include "epsilon.hpp"
#endif

#ifndef GTB_C_MATH_INCLUDED
#include <math.h>
#define GTB_C_MATH_INCLUDED
#endif

#ifndef GTB_C_FLOAT_INCLUDED
#include <float.h>
#define GTB_C_FLOAT_INCLUDED
#endif

GTB_BEGIN_NAMESPACE


class Matrix_4d {
public:
	Matrix_4d();

	explicit Matrix_4d(float m[4][4]);
	explicit Matrix_4d(float m[3][3]);
	explicit Matrix_4d(float m[16]);

	explicit Matrix_4d(double m[4][4]);
	explicit Matrix_4d(double m[16]);

	Matrix_4d(double m00, double m01, double m02, double m03,
		  double m10, double m11, double m12, double m13,
		  double m20, double m21, double m22, double m23,
		  double m30, double m31, double m32, double m33);

	Matrix_4d(const Matrix_4d &m);
	Matrix_4d &operator=(const Matrix_4d &m);

	bool eps_is_equal(const Matrix_4d &m, double eps) const;

	Matrix_4d &reset(double m00, double m01, double m02, double m03,
			 double m10, double m11, double m12, double m13,
			 double m20, double m21, double m22, double m23,
			 double m30, double m31, double m32, double m33);

	Matrix_4d &make_identity();
	Matrix_4d &negate();

	Matrix_4d transpose() const;
	Matrix_4d inverse() const;
	double det() const;

	const double &operator()(unsigned i, unsigned j) const;
	double &operator()(unsigned i, unsigned j);

	Matrix_4d operator-() const;
	Matrix_4d &operator+=(const Matrix_4d &m);
	Matrix_4d &operator-=(const Matrix_4d &m);
	Matrix_4d &operator*=(const Matrix_4d &m);
	Matrix_4d &operator*=(double a);

	friend Matrix_4d operator+(const Matrix_4d &m1, const Matrix_4d &m2);
	friend Matrix_4d operator-(const Matrix_4d &m1, const Matrix_4d &m2);
	friend Matrix_4d operator*(const Matrix_4d &m1, const Matrix_4d &m2);
	friend Matrix_4d operator*(const double a, const Matrix_4d &m);
	friend Matrix_4d operator*(const Matrix_4d &m, double a);

	bool jacobi(float *d, double **v, int &nrot);

	bool LU_decomposition(int p[4]);

	void LU_back_substitution(const int p[4],
				  Matrix_4d &b,
				  Matrix_4d &x);
protected:
	void rot(const double s, const double tau, const int i,
			    const int j, const int k, const int l);

	double m_m[4][4];
};


extern const Matrix_4d MATRIX_4D_ZERO;
extern const Matrix_4d MATRIX_4D_IDENTITY;


inline Matrix_4d::Matrix_4d()
{
}


inline bool Matrix_4d::eps_is_equal(const Matrix_4d &m, double eps) const
{
	return (gtb::eps_is_equal(m_m[0][0], m.m_m[0][0], eps) &&
		gtb::eps_is_equal(m_m[0][1], m.m_m[0][1], eps) &&
		gtb::eps_is_equal(m_m[0][2], m.m_m[0][2], eps) &&
		gtb::eps_is_equal(m_m[0][3], m.m_m[0][3], eps) &&
		
		gtb::eps_is_equal(m_m[1][0], m.m_m[1][0], eps) &&
		gtb::eps_is_equal(m_m[1][1], m.m_m[1][1], eps) &&
		gtb::eps_is_equal(m_m[1][2], m.m_m[1][2], eps) &&
		gtb::eps_is_equal(m_m[1][3], m.m_m[1][3], eps) &&
		
		gtb::eps_is_equal(m_m[2][0], m.m_m[2][0], eps) &&
		gtb::eps_is_equal(m_m[2][1], m.m_m[2][1], eps) &&
		gtb::eps_is_equal(m_m[2][2], m.m_m[2][2], eps) &&
		gtb::eps_is_equal(m_m[2][3], m.m_m[2][3], eps) &&
		
		gtb::eps_is_equal(m_m[3][0], m.m_m[3][0], eps) &&
		gtb::eps_is_equal(m_m[3][1], m.m_m[3][1], eps) &&
		gtb::eps_is_equal(m_m[3][2], m.m_m[3][2], eps) &&
		gtb::eps_is_equal(m_m[3][3], m.m_m[3][3], eps));
}


inline Matrix_4d &Matrix_4d::reset(
	double m00, double m01, double m02, double m03,
	double m10, double m11, double m12, double m13,
	double m20, double m21, double m22, double m23,
	double m30, double m31, double m32, double m33)
{
	m_m[0][0] = m00;
	m_m[0][1] = m01;
	m_m[0][2] = m02;
	m_m[0][3] = m03;

	m_m[1][0] = m10;
	m_m[1][1] = m11;
	m_m[1][2] = m12;
	m_m[1][3] = m13;

	m_m[2][0] = m20;
	m_m[2][1] = m21;
	m_m[2][2] = m22;
	m_m[2][3] = m23;

	m_m[3][0] = m30;
	m_m[3][1] = m31;
	m_m[3][2] = m32;
	m_m[3][3] = m33;

	return *this;
}


inline Matrix_4d Matrix_4d::transpose() const
{
	return Matrix_4d(m_m[0][0], m_m[1][0], m_m[2][0], m_m[3][0],
			 m_m[0][1], m_m[1][1], m_m[2][1], m_m[3][1],
			 m_m[0][2], m_m[1][2], m_m[2][2], m_m[3][2],
			 m_m[0][3], m_m[1][3], m_m[2][3], m_m[3][3]);
}


inline const double &Matrix_4d::operator()(unsigned i, unsigned j) const
{
	GTB_REQUIRE(i < 4);
	GTB_REQUIRE(j < 4);
	return m_m[i][j];
}


inline double &Matrix_4d::operator()(unsigned i, unsigned j)
{
	GTB_REQUIRE(i < 4);
	GTB_REQUIRE(j < 4);
	return m_m[i][j];
}


inline Matrix_4d Matrix_4d::operator-() const
{
	return Matrix_4d(-m_m[0][0], -m_m[0][1], -m_m[0][2], -m_m[0][3],
			 -m_m[1][0], -m_m[1][1], -m_m[1][2], -m_m[1][3],
			 -m_m[2][0], -m_m[2][1], -m_m[2][2], -m_m[2][3],
			 -m_m[3][0], -m_m[3][1], -m_m[3][2], -m_m[3][3]);
}


inline Matrix_4d &Matrix_4d::operator+=(const Matrix_4d &m)
{
	m_m[0][0] += m.m_m[0][0];
	m_m[0][1] += m.m_m[0][1];
	m_m[0][2] += m.m_m[0][2];
	m_m[0][3] += m.m_m[0][3];

	m_m[1][0] += m.m_m[1][0];
	m_m[1][1] += m.m_m[1][1];
	m_m[1][2] += m.m_m[1][2];
	m_m[1][3] += m.m_m[1][3];

	m_m[2][0] += m.m_m[2][0];
	m_m[2][1] += m.m_m[2][1];
	m_m[2][2] += m.m_m[2][2];
	m_m[2][3] += m.m_m[2][3];

	m_m[3][0] += m.m_m[3][0];
	m_m[3][1] += m.m_m[3][1];
	m_m[3][2] += m.m_m[3][2];
	m_m[3][3] += m.m_m[3][3];

	return *this;
}


inline Matrix_4d &Matrix_4d::operator-=(const Matrix_4d &m)
{
	m_m[0][0] -= m.m_m[0][0];
	m_m[0][1] -= m.m_m[0][1];
	m_m[0][2] -= m.m_m[0][2];
	m_m[0][3] -= m.m_m[0][3];

	m_m[1][0] -= m.m_m[1][0];
	m_m[1][1] -= m.m_m[1][1];
	m_m[1][2] -= m.m_m[1][2];
	m_m[1][3] -= m.m_m[1][3];

	m_m[2][0] -= m.m_m[2][0];
	m_m[2][1] -= m.m_m[2][1];
	m_m[2][2] -= m.m_m[2][2];
	m_m[2][3] -= m.m_m[2][3];

	m_m[3][0] -= m.m_m[3][0];
	m_m[3][1] -= m.m_m[3][1];
	m_m[3][2] -= m.m_m[3][2];
	m_m[3][3] -= m.m_m[3][3];

	return *this;
}


inline Matrix_4d operator*(const Matrix_4d &m1, const Matrix_4d &m2)
{
	Matrix_4d t;

	for (unsigned i = 0; i < 4; i++) {
		for (unsigned j = 0; j < 4; j++) {
			t.m_m[i][j] = 0.0;
			for (unsigned k = 0; k < 4; k++) {
				t.m_m[i][j] += m1.m_m[i][k] * m2.m_m[k][j];
			}
		}
	}
	return t;
}


inline Matrix_4d &Matrix_4d::operator*=(const Matrix_4d &m)
{
	*this = *this * m;
	return *this;
}


inline Matrix_4d &Matrix_4d::operator*=(double a)
{
	m_m[0][0] *= a;
	m_m[0][1] *= a;
	m_m[0][2] *= a;
	m_m[0][3] *= a;

	m_m[1][0] *= a;
	m_m[1][1] *= a;
	m_m[1][2] *= a;
	m_m[1][3] *= a;

	m_m[2][0] *= a;
	m_m[2][1] *= a;
	m_m[2][2] *= a;
	m_m[2][3] *= a;

	m_m[3][0] *= a;
	m_m[3][1] *= a;
	m_m[3][2] *= a;
	m_m[3][3] *= a;

	return *this;
}


inline Matrix_4d operator+(const Matrix_4d &m1, const Matrix_4d &m2)
{
	return Matrix_4d(m1.m_m[0][0] + m2.m_m[0][0],
			 m1.m_m[0][1] + m2.m_m[0][1],
			 m1.m_m[0][2] + m2.m_m[0][2],
			 m1.m_m[0][3] + m2.m_m[0][3],

			 m1.m_m[1][0] + m2.m_m[1][0],
			 m1.m_m[1][1] + m2.m_m[1][1],
			 m1.m_m[1][2] + m2.m_m[1][2],
			 m1.m_m[1][3] + m2.m_m[1][3],

			 m1.m_m[2][0] + m2.m_m[2][0],
			 m1.m_m[2][1] + m2.m_m[2][1],
			 m1.m_m[2][2] + m2.m_m[2][2],
			 m1.m_m[2][3] + m2.m_m[2][3],

			 m1.m_m[3][0] + m2.m_m[3][0],
			 m1.m_m[3][1] + m2.m_m[3][1],
			 m1.m_m[3][2] + m2.m_m[3][2],
			 m1.m_m[3][3] + m2.m_m[3][3]);
}


inline Matrix_4d operator-(const Matrix_4d &m1, const Matrix_4d &m2)
{
	return Matrix_4d(m1.m_m[0][0] - m2.m_m[0][0],
			 m1.m_m[0][1] - m2.m_m[0][1],
			 m1.m_m[0][2] - m2.m_m[0][2],
			 m1.m_m[0][3] - m2.m_m[0][3],

			 m1.m_m[1][0] - m2.m_m[1][0],
			 m1.m_m[1][1] - m2.m_m[1][1],
			 m1.m_m[1][2] - m2.m_m[1][2],
			 m1.m_m[1][3] - m2.m_m[1][3],

			 m1.m_m[2][0] - m2.m_m[2][0],
			 m1.m_m[2][1] - m2.m_m[2][1],
			 m1.m_m[2][2] - m2.m_m[2][2],
			 m1.m_m[2][3] - m2.m_m[2][3],

			 m1.m_m[3][0] - m2.m_m[3][0],
			 m1.m_m[3][1] - m2.m_m[3][1],
			 m1.m_m[3][2] - m2.m_m[3][2],
			 m1.m_m[3][3] - m2.m_m[3][3]);
}


inline Matrix_4d operator*(const double a, const Matrix_4d &m)
{
	return Matrix_4d(
		a * m.m_m[0][0],
		a * m.m_m[0][1],
		a * m.m_m[0][2],
		a * m.m_m[0][3],

		a * m.m_m[1][0],
		a * m.m_m[1][1],
		a * m.m_m[1][2],
		a * m.m_m[1][3],

		a * m.m_m[2][0],
		a * m.m_m[2][1],
		a * m.m_m[2][2],
		a * m.m_m[2][3],

		a * m.m_m[3][0],
		a * m.m_m[3][1],
		a * m.m_m[3][2],
		a * m.m_m[3][3]);
}


inline Matrix_4d operator*(const Matrix_4d &m, double a)
{
	return a * m;
}


GTB_END_NAMESPACE

#endif // GTB_MATRIX_4D_INCLUDED
