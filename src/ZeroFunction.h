#ifndef __ZERO_FUNCTION_H__
#define __ZERO_FUNCTION_H__

#include "Field.h"

namespace cigma {
    class ZeroFunction;
}

class cigma::ZeroFunction : Field
{
public:
    void eval(double *point, double *value);

public:
    int n_dim() { return dim; }
    int n_rank() { return rank; }

public:
    void set_shape(int dim, int rank)
    {
        this->dim = dim;
        this->rank = rank;
    }

public:
    int dim;
    int rank;
};

#endif

