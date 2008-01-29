#ifndef __FIAT_PROXY_H__
#define __FIAT_PROXY_H__

#include "Python.h"
#include "numpy/arrayobject.h"

namespace cigma
{
    class FiatProxy;
}

class cigma::FiatProxy
{
public:
    typedef enum {
        LINE=12,
        TRIANGLE=23,
        QUADRANGLE=24,
        TETRAHDRON=34,
        HEXAHEDRON=38
    } FiatShape;

public:
    FiatProxy();
    ~FiatProxy();

public:
    void initialize();
    void finalize();

public:
    void test(const char *msg, int n);
    void quadrature(int shape, int order, double **x, double **w, int *npts, int *dim);
    void tabulate(int npts, double *pts, double *basis, double *basisDeriv);

public:
    PyObject *module;
    PyObject *module_dict;
    PyObject *test_fn;
    PyObject *quad_fn;
    PyObject *tab_fn;
    PyArrayObject *x;
};

#endif
