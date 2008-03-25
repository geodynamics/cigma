#ifndef __ZERO_FIELD_H__
#define __ZERO_FIELD_H__

#include "Field.h"

namespace cigma {
    class ZeroField;
}

class cigma::ZeroField : Field
{
public:
    ZeroField();
    ~ZeroField();

public:
    void eval(double *point, double *value);

public:
    int n_dim() { return dim; }
    int n_rank() { return rank; }
    FieldType getType() { return USER_FIELD; }

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

