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
    int n_celldim() { return 3; } // XXX: change to 2
    int n_dim() { return 3; }

public:
    void shape(int num, double *points, double *values);
    void grad_shape(int num, double *points, double *values);
    bool interior(double u, double v, double w);
    bool interior2(double x, double y, double z);
};

#endif
