#ifndef __TET_H__
#define __TET_H__

#include "Cell.h"

namespace cigma
{
    class Tet;
}

class cigma::Tet : public Cell
{
public:
    Tet();
    ~Tet();

public:
    int n_nodes() { return 4; }
    int n_celldim() { return 3; }
    int n_dim() { return 3; }
    Geometry geometry() { return TETRAHEDRON; }

public:
    void shape(int num, double *points, double *values);
    void grad_shape(int num, double *points, double *values);
    void xyz2uvw(double xyz[3], double uvw[3]);
    bool interior(double u, double v, double w);
    double volume();
};


#endif
