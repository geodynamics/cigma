#ifndef __RESIDUALS_H__
#define __RESIDUALS_H__

#include "MeshPart.h"

namespace cigma
{
    class Residuals;
}

class cigma::Residuals
{
public:
    Residuals();
    ~Residuals();

public:
    void set_mesh(MeshPart *meshPart);

public:
    void zero_out();
    void update(int e, double cell_residual);
    double L2();

public:
    void write_vtk(const char *filename);

public:
    int nel;
    double *epsilon;
    double global_error;
    MeshPart *meshPart;
};

#endif
