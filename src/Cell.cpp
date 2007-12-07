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

    /*
    double s[3];
    switch (celldim)
    {
    case 3:
    case 2:
    case 1:
    default:
        assert(false);
    };*/

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

    // value[i] = N[0]*dofs[0,i] + N[1]*dofs[1,i] + ... 
    for (int i = 0; i < valdim; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < ndofs; j++)
            sum += dofs[i + valdim*j] * N[j];
        value[i] = sum;
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

