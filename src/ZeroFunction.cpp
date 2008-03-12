#include "ZeroFunction.h"

using namespace cigma;

void ZeroFunction::eval(double *x, double *y)
{
    for (int i = 0; i < rank; i++)
    {
        y[i] = 0.0;
    }
}

