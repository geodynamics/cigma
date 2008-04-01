#ifndef __TRI_H__
#define __TRI_H__

#include "Cell.h"

namespace cigma
{
    class Tri;
}

class cigma::Tri : public Cell
{
public:
    Tri();
    ~Tri();

public:
    int n_nodes() { return 3; }
    int n_celldim() { return 2; }
    int n_dim() { return 3; }
    Geometry geometry() { return TRIANGLE; }

public:
    void shape(int num, double *points, double *values);
    void grad_shape(int num, double *points, double *values);
    //void xyz2uvw(double xyz[3], double uvw[3]);
    bool interior(double u, double v, double w);
    double volume();
};

#endif
