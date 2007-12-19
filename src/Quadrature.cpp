#include "Quadrature.h"

// ---------------------------------------------------------------------------

cigma::Quadrature::Quadrature()
{
    qdim = 0;
    qpts = 0;
    qwts = 0;
}


cigma::Quadrature::~Quadrature()
{
    if (data != 0) delete [] data;
    if (qpts != 0) delete [] qpts;
    if (qwts != 0) delete [] qwts;
}


// ---------------------------------------------------------------------------

void cigma::Quadrature::set_data(Cell *cell, double *quadpts, double *quadwts, int npts)
{
    /* some basic assertions */
    assert(cell != 0);
    assert(quadpts != 0);
    assert(quadwts != 0);
    assert(npts > 0);

    /* clear existing data */
    if (data != 0) delete [] data;
    if (qpts != 0) delete [] qpts;
    if (qwts != 0) delete [] qwts;

    /* set dimensions */
    this->num = npts;
    this->dim = cell->n_celldim();
    this->qdim = cell->n_dim();

    /* allocate new data arrays */
    data = new double[npts * dim];
    qpts = new double[npts * qdim];
    qwts = new double[npts];

    /* copy from quadpts & quadwts */
    int i,j;
    for (i = 0; i < npts; i++)
    {
        qwts[i] = quadwts[i];
        for (j = 0; j < dim; j++)
        {
            int n = dim*i + j;
            data[n] = quadpts[n];
        }
        for (j = 0; j < qdim; j++)
        {
            qpts[qdim*i + j] = 0.0;
        }
    }
}

void cigma::Quadrature::apply_refmap()
{
    assert(cell != 0);
    for (int i = 0; i < num; i++)
    {
        cell->uvw2xyz(&data[dim*i], &qpts[qdim*i]);
    }
}


/*
void cigma::Quadrature::set_quadrature(double *points, double *weights, int npts, int nsd)
{
    nq = npts;
    dim = nsd;

    for (int i = 0; i < nq; i++)
    {
        qwts[i] = weights[i];

        for (int j = 0; j < dim; j++)
        {
            qpts[dim*i + j] = points[dim*i + j];
        }
    }
}


void cigma::Quadrature::get_quadrature(double **points, double **weights, int *npts, int *nsd)
{
    double *pts = new double[nq*dim];
    double *wts = new double[nq];

    for (int i = 0; i < nq; i++)
    {
        wts[i] = qwts[i];

        for (int j = 0; j < dim; j++)
        {
            pts[dim*i+j] = qpts[dim*i+j];
        }
    }

    *points = pts;
    *weights = wts;
    *npts = nq;
    *nsd = dim;
}
*/

// ---------------------------------------------------------------------------
