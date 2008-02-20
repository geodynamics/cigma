#ifndef __RESIDUAL_FIELD_H__
#define __RESIDUAL_FIELD_H__

//#include "Field.h"
#include "MeshPart.h"

namespace cigma
{
    class ResidualField;
}

//
// XXX: do we need to derive from cigma::Field?
//
class cigma::ResidualField
{
public:
    ResidualField();
    ~ResidualField();

public:
    void set_mesh(MeshPart *meshPart);

public:
    void write_vtk(const char *filename);

public:
    int nel;
    double *epsilon;
    MeshPart *meshPart;
};

#endif
