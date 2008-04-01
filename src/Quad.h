#ifndef __QUAD_H__
#define __QUAD_H__

#include "Cell.h"

namespace cigma
{
    class Quad;
}

class cigma::Quad : public Cell
{
public:
    Quad();
    ~Quad();

public:
    int n_nodes() { return 4; }
    int n_celldim() { return 2; }
    int n_dim() { return 3; }
    Geometry geometry() { return QUADRANGLE; }

public:
    void shape(int num, double *points, double *values);
    void grad_shape(int num, double *points, double *values);
    bool interior(double u, double v, double w);
    double volume();
};

#endif
