#include "../FiatProxy.h"
#include <iostream>


int main(int argc, char *argv[])
{
    using namespace cigma;

    FiatProxy *fiat = new FiatProxy();
    fiat->initialize();


    //* run test function first
    {
        char *msg = "hello world";
        fiat->test(msg, 3);
    } // */


    /* test quadrature - note copy semantics
    {
        int shape;
        int order;
        double *x, *w;
        int npts, dim;

        fiat->quadrature(shape, order, &x, &w, &npts, &dim);
    } // */


    fiat->finalize();
    delete fiat;

    return 0;
}
