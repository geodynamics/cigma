#ifndef __DOF_HANDLER_H__
#define __DOF_HANDLER_H__

#include "MeshPart.h"

namespace cigma
{
    class DofHandler;
}


/*
 * DofHandler - Container for field data defined over a MeshPart
 */
class cigma::DofHandler
{
public:
    DofHandler();
    ~DofHandler();

public:
    int rank() const;

public:
    void set_data(double *dofs, int nno, int ndim);
    void get_data(int num, int *nodes, double *d);

public:
    int nno;
    int ndim;
    double *dofs;
    MeshPart *meshPart;
};


//----------------------------------------------------------------------------

inline int cigma::DofHandler::rank() const
{
    return ndim;
}

#endif
