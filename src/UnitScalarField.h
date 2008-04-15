#ifndef __UNIT_SCALAR_FIELD_H__
#define __UNIT_SCALAR_FIELD_H__

#include "UserField.h"

namespace cigma
{
    class UnitScalarField;
}

class cigma::UnitScalarField : public cigma::UserField
{
public:
    UnitScalarField();
    ~UnitScalarField();

public:
    int n_dim() { return 3; }
    int n_rank() { return 1; }

public:
    bool eval(double *point, double *value);
};

#endif

