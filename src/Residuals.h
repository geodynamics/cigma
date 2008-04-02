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
    void reset_accumulator();
    void update(int e, double cell_residual);
    double L2();
    double max();

public:
    void write(const char *filename);

public:
    int nel;
    double *epsilon;
    double global_error;
    double max_residual;
    MeshPart *meshPart;
};

#endif
