#include "QuadratureRule.h"


cigma::QuadratureRule::QuadratureRule()
{
    nq = 0;
    dim = 0;
    qpts = 0;
    qwts = 0;
}


cigma::QuadratureRule::~QuadratureRule()
{
}


void cigma::QuadratureRule::set_quadrature(double *points, double *weights, int npts, int nsd)
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


void cigma::QuadratureRule::get_quadrature(double **points, double **weights, int *npts, int *nsd)
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

