#ifndef GTB_MATRIX_4D_INCLUDED
#include "Matrix_4d.hpp"
#endif

GTB_BEGIN_NAMESPACE


const Matrix_4d MATRIX_4D_ZERO(0.0, 0.0, 0.0, 0.0,
			       0.0, 0.0, 0.0, 0.0,
			       0.0, 0.0, 0.0, 0.0,
			       0.0, 0.0, 0.0, 0.0);


const Matrix_4d MATRIX_4D_IDENTITY(1.0, 0.0, 0.0, 0.0,
				   0.0, 1.0, 0.0, 0.0,
				   0.0, 0.0, 1.0, 0.0,
				   0.0, 0.0, 0.0, 1.0);


Matrix_4d::Matrix_4d(float m[4][4])
{
	m_m[0][0] = m[0][0];
	m_m[0][1] = m[0][1];
	m_m[0][2] = m[0][2];
	m_m[0][3] = m[0][3];

	m_m[1][0] = m[1][0];
	m_m[1][1] = m[1][1];
	m_m[1][2] = m[1][2];
	m_m[1][3] = m[1][3];

	m_m[2][0] = m[2][0];
	m_m[2][1] = m[2][1];
	m_m[2][2] = m[2][2];
	m_m[2][3] = m[2][3];

	m_m[3][0] = m[3][0];
	m_m[3][1] = m[3][1];
	m_m[3][2] = m[3][2];
	m_m[3][3] = m[3][3];
}


Matrix_4d::Matrix_4d(float m[3][3])
{
        m_m[0][0] = m[0][0];
        m_m[0][1] = m[0][1];
        m_m[0][2] = m[0][2];
        m_m[0][3] = 0.0;
                                                                                                                   
        m_m[1][0] = m[1][0];
        m_m[1][1] = m[1][1];
        m_m[1][2] = m[1][2];
        m_m[1][3] = 0.0;
                                                                                                                   
        m_m[2][0] = m[2][0];
        m_m[2][1] = m[2][1];
        m_m[2][2] = m[2][2];
        m_m[2][3] = 0.0;
                                                                                                                   
        m_m[3][0] = 0.0;
        m_m[3][1] = 0.0;
        m_m[3][2] = 0.0;
        m_m[3][3] = 1.0;
}


Matrix_4d::Matrix_4d(float m[16])
{
	m_m[0][0] = m[0];
	m_m[0][1] = m[1];
	m_m[0][2] = m[2];
	m_m[0][3] = m[3];

	m_m[1][0] = m[4];
	m_m[1][1] = m[5];
	m_m[1][2] = m[6];
	m_m[1][3] = m[7];

	m_m[2][0] = m[8];
	m_m[2][1] = m[9];
	m_m[2][2] = m[10];
	m_m[2][3] = m[11];

	m_m[3][0] = m[12];
	m_m[3][1] = m[13];
	m_m[3][2] = m[14];
	m_m[3][3] = m[15];
}


Matrix_4d::Matrix_4d(double m[4][4])
{
	m_m[0][0] = m[0][0];
	m_m[0][1] = m[0][1];
	m_m[0][2] = m[0][2];
	m_m[0][3] = m[0][3];

	m_m[1][0] = m[1][0];
	m_m[1][1] = m[1][1];
	m_m[1][2] = m[1][2];
	m_m[1][3] = m[1][3];

	m_m[2][0] = m[2][0];
	m_m[2][1] = m[2][1];
	m_m[2][2] = m[2][2];
	m_m[2][3] = m[2][3];

	m_m[3][0] = m[3][0];
	m_m[3][1] = m[3][1];
	m_m[3][2] = m[3][2];
	m_m[3][3] = m[3][3];
}


Matrix_4d::Matrix_4d(double m[16])
{
	m_m[0][0] = m[0];
	m_m[0][1] = m[1];
	m_m[0][2] = m[2];
	m_m[0][3] = m[3];

	m_m[1][0] = m[4];
	m_m[1][1] = m[5];
	m_m[1][2] = m[6];
	m_m[1][3] = m[7];

	m_m[2][0] = m[8];
	m_m[2][1] = m[9];
	m_m[2][2] = m[10];
	m_m[2][3] = m[11];

	m_m[3][0] = m[12];
	m_m[3][1] = m[13];
	m_m[3][2] = m[14];
	m_m[3][3] = m[15];
}


Matrix_4d::Matrix_4d(double m00, double m01, double m02, double m03,
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
}


Matrix_4d::Matrix_4d(const Matrix_4d &m)
{
	m_m[0][0] = m.m_m[0][0];
	m_m[0][1] = m.m_m[0][1];
	m_m[0][2] = m.m_m[0][2];
	m_m[0][3] = m.m_m[0][3];

	m_m[1][0] = m.m_m[1][0];
	m_m[1][1] = m.m_m[1][1];
	m_m[1][2] = m.m_m[1][2];
	m_m[1][3] = m.m_m[1][3];

	m_m[2][0] = m.m_m[2][0];
	m_m[2][1] = m.m_m[2][1];
	m_m[2][2] = m.m_m[2][2];
	m_m[2][3] = m.m_m[2][3];

	m_m[3][0] = m.m_m[3][0];
	m_m[3][1] = m.m_m[3][1];
	m_m[3][2] = m.m_m[3][2];
	m_m[3][3] = m.m_m[3][3];
}


Matrix_4d &Matrix_4d::operator=(const Matrix_4d &m)
{
	if (&m != this) {
		m_m[0][0] = m.m_m[0][0];
		m_m[0][1] = m.m_m[0][1];
		m_m[0][2] = m.m_m[0][2];
		m_m[0][3] = m.m_m[0][3];

		m_m[1][0] = m.m_m[1][0];
		m_m[1][1] = m.m_m[1][1];
		m_m[1][2] = m.m_m[1][2];
		m_m[1][3] = m.m_m[1][3];

		m_m[2][0] = m.m_m[2][0];
		m_m[2][1] = m.m_m[2][1];
		m_m[2][2] = m.m_m[2][2];
		m_m[2][3] = m.m_m[2][3];

		m_m[3][0] = m.m_m[3][0];
		m_m[3][1] = m.m_m[3][1];
		m_m[3][2] = m.m_m[3][2];
		m_m[3][3] = m.m_m[3][3];
	}
	return *this;
}


Matrix_4d &Matrix_4d::make_identity()
{
	m_m[0][0] = 1.0;
	m_m[0][1] = 0.0;
	m_m[0][2] = 0.0;
	m_m[0][3] = 0.0;

	m_m[1][0] = 0.0;
	m_m[1][1] = 1.0;
	m_m[1][2] = 0.0;
	m_m[1][3] = 0.0;

	m_m[2][0] = 0.0;
	m_m[2][1] = 0.0;
	m_m[2][2] = 1.0;
	m_m[2][3] = 0.0;

	m_m[3][0] = 0.0;
	m_m[3][1] = 0.0;
	m_m[3][2] = 0.0;
	m_m[3][3] = 1.0;

	return *this;
}


Matrix_4d &Matrix_4d::negate()
{
	m_m[0][0] = -m_m[0][0];
	m_m[0][1] = -m_m[0][1];
	m_m[0][2] = -m_m[0][2];
	m_m[0][3] = -m_m[0][3];

	m_m[1][0] = -m_m[1][0];
	m_m[1][1] = -m_m[1][1];
	m_m[1][2] = -m_m[1][2];
	m_m[1][3] = -m_m[1][3];

	m_m[2][0] = -m_m[2][0];
	m_m[2][1] = -m_m[2][1];
	m_m[2][2] = -m_m[2][2];
	m_m[2][3] = -m_m[2][3];

	m_m[3][0] = -m_m[3][0];
	m_m[3][1] = -m_m[3][1];
	m_m[3][2] = -m_m[3][2];
	m_m[3][3] = -m_m[3][3];

	return *this;
}


bool Matrix_4d::LU_decomposition(int p[4])
{
	int i, j, k;

	for (j = 0; j < 3; j++) {

		// Find line of pivot
		p[j] = j;
		for (i = j + 1; i < 4; i++) {
			if (fabs(m_m[i][j]) > fabs(m_m[p[j]][j])) {
				p[j] = i;
			}
		}

		// Swap lines if necessary
		if (p[j] != j) {
			i = p[j];
			for (k = 0; k < 4; k++) {
				double t = m_m[i][k];
				m_m[i][k] = m_m[j][k];
				m_m[j][k] = t;
			}
		}

		// Check if matrix is singular
		if (gtb::eps_is_zero(m_m[j][j], DBL_EPSILON)) {
//			GTB_WARNING("singular matrix");
			return false;
		}

		// Eliminate elements below diagonal
		for (i = j + 1; i < 4; i++) {
			m_m[i][j] = -m_m[i][j] / m_m[j][j];
			for (k = j + 1; k < 4; k++) {
				m_m[i][k] += m_m[i][j] * m_m[j][k];
			}
		}
	}
	return true;
}


void Matrix_4d::LU_back_substitution(const int p[4],
				     Matrix_4d &b,
				     Matrix_4d &x)
{
	int i, j, k;

	// Swap lines of b when necessary
	for (i = 0; i < 3; i++) {
		if (p[i] != i) {
			for (j = 0, k = p[i]; j < 4; j++) {
				double t = b.m_m[i][j];
				b.m_m[i][j] = b.m_m[k][j];
				b.m_m[k][j] = t;
			}
		}
	}

	// Apply multipliers to b
	for (j = 0; j < 3 ; j++) {
		for (i = j + 1; i < 4; i++) {
			for (k = 0; k < 4; k++) {
				b.m_m[i][k] += m_m[i][j] * b.m_m[j][k];
			}
		}
	}

	// Back substitution
	for (k = 0; k < 4; k++) {
		for (i = 3; i >= 0; i--) {
			x.m_m[i][k] = b.m_m[i][k];
			for (j = i + 1; j < 4; j++) {
				x.m_m[i][k] -= m_m[i][j] * x.m_m[j][k];
			}
			GTB_CHECK(!gtb::eps_is_zero(m_m[i][i], DBL_EPSILON));
			x.m_m[i][k] /= m_m[i][i];
		}
	}
}


Matrix_4d Matrix_4d::inverse() const
{
	Matrix_4d a(*this);
	Matrix_4d b(MATRIX_4D_IDENTITY);
	Matrix_4d x;
	int p[4];

	a.LU_decomposition(p);
	a.LU_back_substitution(p, b, x);
	return x;
}


double Matrix_4d::det() const
{
	double d;
	Matrix_4d t(*this);
	int p[4];

	t.LU_decomposition(p);
	d = 1.0;
	for (int i = 0; i < 3; i++) {
		d *= t.m_m[i][i];
		if (p[i] != i) {
			d = -d;
		}
	}
	d *= t.m_m[3][3];
	return d;
}

inline
void Matrix_4d::rot(const double s, const double tau, const int i,
		    const int j, const int k, const int l)
{
	double g,h;

	g = m_m[i][j];
	h = m_m[k][l];
	m_m[i][j] = g-s*(h+g*tau);
	m_m[k][l] = h+s*(g-h*tau);
}

bool Matrix_4d::jacobi(float *d, double **v, int &nrot)
{
	int i,j,ip,iq;
	double tresh,theta,tau,t,sm,s,h,g,c;

	int n=4;
	double b[4];
	double z[4];
	for (ip=0; ip<n; ip++) {
		for (iq=0; iq<n; iq++) v[ip][iq] = 0.0;
		v[ip][ip] = 1.0;
	}
	for (ip=0; ip<n; ip++) {
		b[ip] = d[ip] = m_m[ip][ip];
		z[ip] = 0.0;
	}
	nrot = 0;
	for (i=1; i<= 50; i++) {
		sm = 0.0;
		// sum magnitude of off-diagonal elements
		for (ip=0; ip<n-1; ip++) {
			for (iq=ip+1; iq<n; iq++)
				sm += fabs(m_m[ip][iq]);
		}
		if (sm == 0.0) {
			// relies on quadric convergence to machine underflow
			return true;
		}
		if (i < 4)
			tresh = 0.2*sm/(n*n);
		else
			tresh = 0.0;
		for (ip=0; ip<n-1; ip++) {
			for (iq=ip+1; iq<n; iq++) {
				g=100.0 * fabs(m_m[i][iq]);
				// after 4 sweeps, skip the rotation if the off-diagonal element is small.
				if (i > 4 && (fabs(d[ip])+g) == fabs(d[ip])
				    && (fabs(d[iq])+g) == fabs(d[iq]))
					m_m[ip][iq] = 0.0;
				else if (fabs(m_m[ip][iq]) > tresh) {
					h = d[iq]-d[ip];
					if ((fabs(h)+g) == fabs(h)) {
						t = (m_m[ip][iq])/h;
					} else {
						theta = 0.5*h/(m_m[ip][iq]);
						t = 1.0/(fabs(theta)+sqrt(1.0+theta*theta));
						if (theta < 0.0) t = -t;
					}
					c = 1.0/sqrt(1+t*t);
					s = t*c;
					tau = s/(1.0+c);
					h=t*m_m[ip][iq];
					z[ip] -= h;
					z[iq] += h;
					d[ip] -= h;
					d[iq] += h;
					m_m[ip][iq] = 0.0;
					for (j=0; j<ip; j++)
						rot(s, tau, j, ip, j, iq);
					for (j=ip+1; j<iq; j++)
						rot(s, tau, ip, j, j, iq);
					for (j=iq+1; i<n; j++)
						rot(s, tau, ip, j, iq, j);
					for (j=0; j<n; j++)
						rot(s, tau, j, ip, j, iq);
					++nrot;
				}
			}
		}
		for (ip=0; ip<n; ip++) {
			b[ip] += z[ip];
			d[ip] = b[ip];
			z[ip] = 0.0;
		}
	}
	// Too many iterations in routine jacobi
	return false;
}


GTB_END_NAMESPACE
