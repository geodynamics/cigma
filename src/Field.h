#ifndef __FIELD_H__
#define __FIELD_H__

namespace cigma
{
    class Field;
}


/*
 * Base Field object
 */
class cigma::Field
{
public:
    virtual void eval(double *point, double *value) = 0;
    virtual ~Field();
};

#endif
