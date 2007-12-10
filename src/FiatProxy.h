#ifndef __FIAT_PROXY_H__
#define __FIAT_PROXY_H__

#include "Python.h"
#include "arrayobject.h"

namespace cigma
{
    class FiatProxy;
}

class cigma::FiatProxy
{
public:
    FiatProxy();
    ~FiatProxy();

public:
    void initialize();
    void finalize();

public:
    void quadrature(int shape, int order, double **x, double **w, int *npts, int *dim);
    void tabulate(int npts, double *pts, double *basis, double *basisDeriv);

public:
    PyObject *module;
    PyObject *module_dict;
    PyObject *quad_fn;
    PyObject *tab_fn;
    PyArrayObject *x;
};

#endif
