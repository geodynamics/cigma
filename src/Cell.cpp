#include "Cell.h"
#include <cstdlib>
#include <cassert>


cigma::Cell::Cell()
{
    nsd = -1;
    celldim = -1;

    nno = 0;
    globverts = NULL;
    refverts = NULL;

    ndofs = 0;
    basis_tab = NULL;
    basis_jet = NULL;

    nq = 0;
    jxw = NULL;
    qpts = NULL;
    qwts = NULL;
    gqpts = NULL;

    _tabulate = false;
}


cigma::Cell::~Cell()
{

}

//----------------------------------------------------------------------------


void cigma::Cell::clear(void)
{
    globverts = NULL;
}


void cigma::Cell::set(int nsd, int celldim, int ndofs)
{
    this->nsd = nsd;
    this->celldim = celldim;
    this->ndofs = ndofs;
}


void cigma::Cell::set_reference_vertices(const double *vertices, int num_vertices)
{
    assert(nsd > 0);
    assert(celldim > 0);

    nno = num_vertices;
    refverts = new double[nno*celldim];
    for (int i = 0; i < nno; i++)
    {
        for(int j = 0; j < celldim; j++)
        {
            int n = celldim*i + j;
            refverts[n] = vertices[n];
        }
    }
    globverts = refverts;
}


void cigma::Cell::update_vertices(double *vertices, int num_vertices, int nsd)
{
    assert((this->nsd) == nsd);
    assert(nno == num_vertices);
    this->globverts = vertices;
}


void cigma::Cell::set_quadrature(const double *quadpts, const double *quadwts, int num_points)
{
    assert(nsd > 0);
    nq = num_points;
    qwts = new double[nq];
    qpts = new double[nq*celldim];
    gqpts = new double[nq*nsd];
    for (int q = 0; q < nq; q++)
    {
        qwts[q] = quadwts[q];
        for (int i = 0; i < celldim; i++)
        {
            int n = celldim*q + i;
            qpts[n] = quadpts[n];
        }
    }
    if (basis_tab != NULL)
    {
        update_quadrature();
    }
}


void cigma::Cell::update_quadrature(void)
{
    assert(nsd > 0);
    assert(nq > 0);
    for (int q = 0; q < nq; q++)
        uvw2xyz(&qpts[celldim*q], &gqpts[nsd*q]);
}


//----------------------------------------------------------------------------


double cigma::Cell::jacobian(double *point, double jac[3][3])
{
    assert(celldim > 0);
    switch (celldim)
    {
    case 3: return jacobian(point[0], point[1], point[2], jac);
    case 2: return jacobian(point[0], point[1], 0.0, jac);
    case 1: return jacobian(point[0], 0.0, 0.0, jac);
    }
    assert(false);
    return 0.0;
}

double cigma::Cell::jacobian(double u, double v, double w, double jac[3][3])
{
    jac[0][0] = jac[0][1] = jac[0][2] = 0.0;
    jac[1][0] = jac[1][1] = jac[1][2] = 0.0;
    jac[2][0] = jac[2][1] = jac[2][2] = 0.0;

    double uvw[3] = {u,v,w};
    double *grad = new double[ndofs*3];
    double *s;

    #define X(i)  globverts[3*(i) + 0]
    #define Y(i)  globverts[3*(i) + 1]
    #define Z(i)  globverts[3*(i) + 2]

    switch (celldim)
    {
        case 3 :

            grad_shape(1, uvw, grad);

            for(int i = 0; i < n_nodes(); i++)
            {
                s = &grad[3*i];

                jac[0][0] += X(i) * s[0];
                jac[0][1] += Y(i) * s[0];
                jac[0][2] += Z(i) * s[0];

                jac[1][0] += X(i) * s[1];
                jac[1][1] += Y(i) * s[1];
                jac[1][2] += Z(i) * s[1];

                jac[2][0] += X(i) * s[2];
                jac[2][1] += Y(i) * s[2];
                jac[2][2] += Z(i) * s[2];
            }
            return fabs(
                    + jac[0][0] * jac[1][1] * jac[2][2]
                    + jac[0][2] * jac[1][0] * jac[2][1]
                    + jac[0][1] * jac[1][2] * jac[2][0]
                    - jac[0][2] * jac[1][1] * jac[2][0]
                    - jac[0][0] * jac[1][2] * jac[2][1]
                    - jac[0][1] * jac[1][0] * jac[2][2]);

        case 2 :

            grad_shape(1, uvw, grad);

            for(int i = 0; i < n_nodes(); i++)
            {
                s = &grad[2*i];

                jac[0][0] += X(i) * s[0];
                jac[0][1] += Y(i) * s[0];
                jac[0][2] += Z(i) * s[0];

                jac[1][0] += X(i) * s[1];
                jac[1][1] += Y(i) * s[1];
                jac[1][2] += Z(i) * s[1];
            }
            {
                double a[3], b[3], c[3];

                a[0]= X(1) - X(0);
                a[1]= Y(1) - Y(0);
                a[2]= Z(1) - Z(0);

                b[0]= X(2) - X(0);
                b[1]= Y(2) - Y(0);
                b[2]= Z(2) - Z(0);

                prodve(a, b, c);

                jac[2][0] = c[0];
                jac[2][1] = c[1];
                jac[2][2] = c[2];
            }
            return sqrt(SQR(jac[0][0] * jac[1][1] - jac[0][1] * jac[1][0]) +
                        SQR(jac[0][2] * jac[1][0] - jac[0][0] * jac[1][2]) +
                        SQR(jac[0][1] * jac[1][2] - jac[0][2] * jac[1][1]));

        case 1:

            grad_shape(1, uvw, grad);

            for(int i = 0; i < n_nodes(); i++)
            {
                s = &grad[i];

                jac[0][0] += X(i) * s[0];
                jac[0][1] += Y(i) * s[0];
                jac[0][2] += Z(i) * s[0];
            }
            {
                double a[3], b[3], c[3];
                a[0]= X(1) - X(0);
                a[1]= Y(1) - Y(0);
                a[2]= Z(1) - Z(0);

                if((fabs(a[0]) >= fabs(a[1]) && fabs(a[0]) >= fabs(a[2])) ||
                   (fabs(a[1]) >= fabs(a[0]) && fabs(a[1]) >= fabs(a[2])))
                {
                    b[0] =  a[1];
                    b[1] = -a[0];
                    b[2] =  0.0;
                }
                else
                {
                    b[0] =  0.0;
                    b[1] =  a[2];
                    b[2] = -a[1];
                }

                prodve(a, b, c);

                jac[1][0] = b[0];
                jac[1][1] = b[1];
                jac[1][2] = b[2];

                jac[2][0] = c[0];
                jac[2][1] = c[1];
                jac[2][2] = c[2];
          }
          return sqrt(SQR(jac[0][0])+SQR(jac[0][1])+SQR(jac[0][2]));
    }
    #undef X
    #undef Y
    #undef Z

    return 0.0;
}


/*
 * \tilde{\phi}(x) = \sum_{i} d_i N_i(\xi(x))
 * @param valdim dimension of value points
 * @param dofs dofs is a rank-2 array of dimension [ndofs x valdim]
 * @param point point is a rank-1 array of dimension [celldim]
 * @param value value is a rank-1 array of dimension [valdim]
 */
void cigma::Cell::interpolate(double *dofs, double *point, double *value, int valdim)
{
    double N[ndofs];

    shape(1, point, N);

    // 
    // value[i] = N[0]*dofs[0,i] + N[1]*dofs[1,i] + ... 
    //
    for (int i = 0; i < valdim; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < ndofs; j++)
            sum += dofs[i + valdim*j] * N[j];
        value[i] = sum;
    }
}


/*
 * @param dofs is a rank-2 array of dimension [ndofs x valdim]
 * @param point is a rank-1 array of dimension [celldim]
 * @param value is a rank-1 array of dimension [valdim]
 */
void cigma::Cell::interpolate_grad(double *dofs, double *point, double *value, int stride, double invjac[3][3])
{
    assert(celldim > 0);
    assert(nno > 0);
    assert(ndofs > 0);
    
    int i, j, k;

    double dfdu[3] = {0.0, 0.0, 0.0};
    double grad[ndofs*3];
    double *s;

    grad_shape(1, point, grad);

    k = 0;
    for (i = 0; i < ndofs; i++)
    {
        s = &grad[3*i];
        for (j = 0; j < celldim; j++)
        {
            dfdu[j] += dofs[k] * s[j];
        }
        k += stride;
    }

    if (invjac)
    {
        matvec(invjac, dfdu, value);
    }
    else
    {
        double jac[3][3], inv[3][3];
        jacobian(point, jac);
        inv3x3(jac, inv);
        matvec(inv, dfdu, value);
    }
}


/*
 * Inverse reference map
 */
void cigma::Cell::xyz2uvw(double xyz[3], double uvw[3])
{
    // general newton routine for the nonlinear case
    uvw[0] = uvw[1] = uvw[2] = 0.0;

    int iter = 1, maxiter = 20;
    double error = 1.0, tol = 1.0e-6;

    while ((error > tol) && (iter < maxiter))
    {
        double jac[3][3];
        if (!jacobian(uvw[0], uvw[1], uvw[2], jac))
            break;

        double xn = 0.0, yn = 0.0, zn = 0.0;


        double s[ndofs];

        shape(1, uvw, s);

        #define X(i)  globverts[3*(i) + 0]
        #define Y(i)  globverts[3*(i) + 1]
        #define Z(i)  globverts[3*(i) + 2]

        for (int i = 0; i < nno; i++)
        {
            xn += X(i) * s[i];
            yn += Y(i) * s[i];
            zn += Z(i) * s[i];
        }

        #undef X
        #undef Y
        #undef Z

        double inv[3][3];
        inv3x3(jac, inv);

        double un = uvw[0] + inv[0][0] * (xyz[0] - xn)
                           + inv[1][0] * (xyz[1] - yn)
                           + inv[2][0] * (xyz[2] - zn);

        double vn = uvw[1] + inv[0][1] * (xyz[0] - xn)
                           + inv[1][1] * (xyz[1] - yn)
                           + inv[2][1] * (xyz[2] - zn);

        double wn = uvw[2] + inv[0][2] * (xyz[0] - xn)
                           + inv[1][2] * (xyz[1] - yn)
                           + inv[2][2] * (xyz[2] - zn);

        error = sqrt(SQR(un - uvw[0]) +
                     SQR(vn - uvw[1]) +
                     SQR(wn - uvw[2]));

        uvw[0] = un;
        uvw[1] = vn;
        uvw[2] = wn;

        iter++;
    }
}


/* isoparametric reference map */
void cigma::Cell::uvw2xyz(double uvw[3], double xyz[3])
{
    interpolate(globverts, uvw, xyz, 3);
}


void cigma::Cell::bbox(double *min, double *max)
{
    minmax(globverts, nno, nsd, min, max);
}


void cigma::Cell::centroid(double c[3])
{
    cigma::centroid(globverts, nno, nsd, c);
}

