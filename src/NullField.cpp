#include "NullField.h"

using namespace cigma;

NullField::NullField() {}
NullField::~NullField() {}
bool NullField::eval(double *point, double *value) { return false; }

