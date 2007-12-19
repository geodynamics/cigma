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
    virtual int n_dim() = 0;
    virtual int n_rank() = 0;
    virtual void eval(double *point, double *value) = 0;
    virtual void eval(Points &domain, Points &range);
    virtual ~Field();
};


#endif
