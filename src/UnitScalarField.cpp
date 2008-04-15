#include "UnitScalarField.h"

using namespace cigma;

UnitScalarField::UnitScalarField() {}
UnitScalarField::~UnitScalarField() {}

bool UnitScalarField::eval(double *point, double *value)
{
    value[0] = 1;
    return true;
}


