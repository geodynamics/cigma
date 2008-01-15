//#include <iostream>
#include <cstdlib>
#include <cassert>
#include "Cell.h"


cigma::Cell::Cell()
{
    //std::cout << "Calling cigma::Cell::Cell()\n";

    /*nno = 0;
    nsd = 0;
    celldim = 0;*/

    globverts = NULL;
    refverts = NULL;

    /*ndofs = 0;
    basis_tab = NULL;
    basis_jet = NULL;*/

    /*nq = 0;
    qpts = NULL;
    qwts = NULL;
    gqpts = NULL;
    jxw = NULL;*/

    //_tabulate = false;
}


cigma::Cell::~Cell()
{
    //std::cout << "Calling cigma::Cell::~Cell()\n";
}


/*
int cigma::Cell::n_nodes() const
{
    return nno;
}

int cigma::Cell::n_dim() const
{
    return nsd;
}

int cigma::Cell::n_celldim() const
{
    return celldim;
}*/

//----------------------------------------------------------------------------


/*
void cigma::Cell::set_dims(int ndofs, int celldim, int nsd)
{
    this->nno = ndofs;
    this->celldim = celldim;
    this->nsd = nsd;
}*/


void cigma::Cell::set_reference_vertices(double *vertices, int num_vertices)
{
    int i,j;

    const int nno = n_nodes();
    const int celldim = n_celldim();
    //const int nsd = n_dim();

    assert(nno == num_vertices);

    if (refverts != 0) delete [] refverts;
    //if (globverts != 0) delete [] globverts;

    refverts = new double[nno*celldim];
    //globverts = new double[nno*nsd];

    for (i = 0; i < nno; i++)
    {
        /*
        for (j = 0; j < nsd; j++)
        {
            globverts[nsd*i + j] = 0.0;
        } // */
        for (j = 0; j < celldim; j++)
        {
            int n = celldim*i + j;
            refverts[n] = vertices[n];
            //globverts[nsd*i+j] = vertices[n];
        }
    }
    globverts = refverts;
}


void cigma::Cell::set_global_vertices(double *vertices, int num, int dim)
{
    assert(num == n_nodes());
    assert(dim == n_dim());

    //* // Copy reference
    globverts = vertices;
    // */

    /* // Copy data
    int i,j;
    for (i = 0; i < nno; i++)
    {
        // zero out globverts
        for (j = 0; j < nsd; j++)
        {
            globverts[nsd*i+j] = 0.0;
        }
        // copy globverts
        for (j = 0; j < dim; j++)
        {
            globverts[nsd*i + j] = vertices[dim*i + j];
        }
    } // */

    return;
}


//----------------------------------------------------------------------------


double cigma::Cell::jacobian(double *point, double jac[3][3])
{
    const int celldim = n_celldim();
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

    const int nno = n_nodes();
    const int celldim = n_celldim();

    double uvw[3] = {u,v,w};
    double *grad = new double[nno*3];
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
    const int nno = n_nodes();
    double N[nno];

    shape(1, point, N);

    // 
    // value[i] = N[0]*dofs[0,i] + N[1]*dofs[1,i] + ... 
    //
    for (int i = 0; i < valdim; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < nno; j++)
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
    const int nno = n_nodes();
    const int celldim = n_celldim();

    //assert(nno > 0);
    //assert(celldim > 0);
    //assert(ndofs > 0); <-- ndofs is (cell->nno)
    
    int i, j, k;

    double dfdu[3] = {0.0, 0.0, 0.0};
    double grad[nno*3];
    double *s;

    grad_shape(1, point, grad);

    k = 0;
    for (i = 0; i < nno; i++)
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

    const int nno = n_nodes();
    int iter = 1, maxiter = 20;
    double error = 1.0, tol = 1.0e-6;

    while ((error > tol) && (iter < maxiter))
    {
        double jac[3][3];
        if (!jacobian(uvw[0], uvw[1], uvw[2], jac))
            break;

        double xn = 0.0, yn = 0.0, zn = 0.0;


        double s[nno];

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
    assert(globverts != 0);
    interpolate(globverts, uvw, xyz, 3);
}


void cigma::Cell::bbox(double *min, double *max)
{
    const int nno = n_nodes();
    const int nsd = n_dim();
    minmax(globverts, nno, nsd, min, max);
}


void cigma::Cell::centroid(double c[3])
{
    const int nno = n_nodes();
    const int nsd = n_dim();
    cigma::centroid(globverts, nno, nsd, c);
}

