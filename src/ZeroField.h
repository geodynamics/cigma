#ifndef __ZERO_FIELD_H__
#define __ZERO_FIELD_H__

#include "UserField.h"

namespace cigma {
    class ZeroField;
}

class cigma::ZeroField : public cigma::UserField
{
public:
    ZeroField();
    ~ZeroField();
    void set_shape(int dim, int rank);

public:
    int n_dim() { return dim; }
    int n_rank() { return rank; }

public:
    bool eval(double *point, double *value);

public:
    int dim;
    int rank;
};

#endif

