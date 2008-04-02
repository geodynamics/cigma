#include "QuadraturePoints.h"

using namespace cigma;

// ---------------------------------------------------------------------------

QuadraturePoints::QuadraturePoints()
{
    qdim = 0;
    qpts = 0;
    qwts = 0;
}


QuadraturePoints::~QuadraturePoints()
{
    clear();
}

void QuadraturePoints::clear()
{
    if (data != 0)
    {
        delete [] data;
        data = 0;
    }
    if (qpts != 0)
    {
        delete [] qpts;
        qpts = 0;
    }
    if (qwts != 0)
    {
        delete [] qwts;
        qwts = 0;
    }
}

// ---------------------------------------------------------------------------

void QuadraturePoints::set_quadrature(double *qwts, double *qpts, int npts, int qdim)
{
    /* some basic assertions */
    assert(qpts != 0);
    assert(qwts != 0);
    assert(npts > 0);
    assert(qdim > 0);

    /* clear existing data */
    this->clear();

    /* set dimensions */
    this->npts = npts;
    this->qdim = qdim;

    /* allocate new data arrays */
    this->qpts = new double[npts * qdim];
    this->qwts = new double[npts];

    /* copy from quadpts & quadwts */
    int i,j;
    for (i = 0; i < npts; i++)
    {
        this->qwts[i] = qwts[i];
        for (j = 0; j < qdim; j++)
        {
            int n = qdim*i + j;
            this->qpts[n] = qpts[n];
        }
    }

    /* global quadrature points */
    ndim = 3;
    data = new double[npts * ndim];
    for (i = 0; i < npts; i++)
    {
        for (j = 0; j < ndim; j++)
        {
            data[ndim*i + j] = 0.0;
        }
    }
}

void QuadraturePoints::apply_refmap(Cell *cell)
{
    assert(cell != 0);
    assert(data != 0);

    for (int i = 0; i < npts; i++)
    {
        double uvw[3] = {0,0,0};

        for (int j = 0; j < qdim; j++)
        {
            uvw[j] = qpts[qdim*i + j];
        }
        
        double *xyz = &data[ndim*i];

        cell->uvw2xyz(uvw, xyz);
    }
}

// ---------------------------------------------------------------------------
