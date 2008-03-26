#include "QuadraturePoints.h"

// ---------------------------------------------------------------------------

cigma::QuadraturePoints::QuadraturePoints()
{
    qdim = 0;
    qpts = 0;
    qwts = 0;
}


cigma::QuadraturePoints::~QuadraturePoints()
{
    if (qpts != 0) delete [] qpts;
    if (qwts != 0) delete [] qwts;
}


// ---------------------------------------------------------------------------

void cigma::QuadraturePoints::set_quadrature(double *quadpts, double *quadwts, int npts, int qdim)
{
    /* some basic assertions */
    assert(quadpts != 0);
    assert(quadwts != 0);
    assert(npts > 0);
    assert(qdim > 0);

    /* clear existing data */
    if (qpts != 0) delete [] qpts;
    if (qwts != 0) delete [] qwts;

    /* set dimensions */
    this->num = npts;
    this->qdim = qdim;

    /* allocate new data arrays */
    qpts = new double[num * qdim];
    qwts = new double[num];

    /* copy from quadpts & quadwts */
    int i,j;
    for (i = 0; i < num; i++)
    {
        qwts[i] = quadwts[i];
        for (j = 0; j < qdim; j++)
        {
            int n = qdim*i + j;
            qpts[n] = quadpts[n];
        }
    }
}

void cigma::QuadraturePoints::set_globaldim(int dim)
{
    assert(num > 0);
    this->dim = dim;
    this->data = new double[num * dim];
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            data[dim*i+j] = 0.0;
        }
    }
}


void cigma::QuadraturePoints::apply_refmap(Cell *cell)
{
    assert(cell != 0);
    assert(data != 0);

    for (int i = 0; i < num; i++)
    {
        double *uvw = &qpts[qdim*i];
        double *xyz = &data[dim*i];
        cell->uvw2xyz(uvw, xyz);
    }
}

// ---------------------------------------------------------------------------
