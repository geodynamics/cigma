#include "ZeroField.h"

using namespace cigma;


ZeroField::ZeroField()
{
    dim = rank = 0;
}


ZeroField::~ZeroField()
{
}

void ZeroField::set_shape(int dim, int rank)
{
    this->dim = dim;
    this->rank = rank;
}

bool ZeroField::eval(double *x, double *y)
{
    for (int i = 0; i < rank; i++)
    {
        y[i] = 0.0;
    }
    return true;
}

