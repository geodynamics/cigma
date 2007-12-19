#include "FiatProxy.h"
#include <iostream>
#include <cassert>

// ---------------------------------------------------------------------------

cigma::FiatProxy::FiatProxy()
{
}

cigma::FiatProxy::~FiatProxy()
{
}

// ---------------------------------------------------------------------------

void cigma::FiatProxy::initialize()
{
    PyObject *module_name;

    Py_Initialize();

    import_array();

    module_name = PyString_FromString("FiatProxy");
    module = PyImport_Import(module_name);
    
    if (!module)
    {
        /* XXX: raise exception */ 
        assert(false);
    }

    module_dict = PyModule_GetDict(module);

    test_fn = PyDict_GetItemString(module_dict, "test");
    if (!test_fn)
    {
        /* XXX: raise exception */
        assert(false);
    }
    if (!PyCallable_Check(test_fn))
    {
        /* XXX: raise exception */
        assert(false);
    }

    quad_fn = PyDict_GetItemString(module_dict, "quadrature");
    if (!quad_fn)
    {
        /* XXX: raise exception */
        assert(false);
    }
    if (!PyCallable_Check(quad_fn))
    {
        /* XXX: raise exception */
        assert(false);
    }

    tab_fn = PyDict_GetItemString(module_dict, "tabulate");
    if (!tab_fn) 
    {
        /* XXX: raise exception */
        assert(false);
    }
    if (!PyCallable_Check(tab_fn))
    {
        /* XXX: raise exception */
        assert(false);
    }

    Py_XDECREF(module_name);
}

void cigma::FiatProxy::finalize()
{
    if (module)
        Py_XDECREF(module);
}

void cigma::FiatProxy::test(const char *msg, int n)
{
    PyObject *result;
    long answer;
    
    result = PyObject_CallFunction(test_fn, "(si)", msg, n);

    if (result)
    {
        answer = PyInt_AsLong(result);

        std::cout << "Answer is " << answer << std::endl;

        Py_XDECREF(result);
    }
}

void cigma::FiatProxy::quadrature(int shape, int order, double **x, double **w, int *npts, int *dim)
{
}

void cigma::FiatProxy::tabulate(int npts, double *pts, double *basis, double *basisDeriv)
{
}

