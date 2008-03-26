#ifndef __NULL_FIELD_H__
#define __NULL_FIELD_H__

#include "Field.h"

namespace cigma
{
    class NullField;
}

class cigma::NullField : public Field
{
public:
    NullField();
    ~NullField();

public:
    int n_dim() { return 0; }
    int n_rank() { return 0; }
    FieldType getType() { return NULL_FIELD; }

public:
    void eval(double *point, double *value);

};

#endif
