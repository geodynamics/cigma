#ifndef __HEX_H__
#define __HEX_H__

#include "Cell.h"

namespace cigma
{
    class Hex;
}

class cigma::Hex : public Cell
{
public:
    Hex();
    ~Hex();

public:
    void shape(int num, double *points, double *values);
    void grad_shape(int num, double *points, double *values);
    bool interior(double u, double v, double w);
};


#endif
