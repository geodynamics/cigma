#ifndef __FIELD_H__
#define __FIELD_H__

#include "Points.h"


namespace cigma
{
    class Field;
}


/**
 * @brief Base Field object
 */
class cigma::Field
{
public:
    typedef enum {
        NULL_FIELD,
        FE_FIELD,
        POINT_FIELD,
        EXT_FIELD,
        USER_FIELD
    } FieldType;

public:
    virtual int n_dim() = 0;
    virtual int n_rank() = 0;
    virtual FieldType getType() = 0;

public:
    virtual bool eval(double *point, double *value) = 0;
    virtual bool eval(Points &domain, Points &range);
    virtual ~Field();
};

cigma::Field *NewField(const char *src);


#endif
