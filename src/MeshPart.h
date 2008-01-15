#ifndef __MESH_PART_H__
#define __MESH_PART_H__

#include "Cell.h"

namespace cigma
{
    class MeshPart;
}

class cigma::MeshPart
{
public:
    MeshPart();
    virtual ~MeshPart();

public:
    //virtual int n_nodes() = 0;
    //virtual int n_nsd() = 0;
    //virtual int n_nel() = 0;
    //virtual int n_ndofs() = 0;

public:
    void set_coordinates(double *coordinates, int nno, int nsd);
    void set_connectivity(int *connectivity, int nel, int ndofs);

public:
    void get_cell_coords(int cellIndex, double *globalCoords);
    virtual bool find_cell(double globalPoint[3], int *cellIndex) = 0;

public:
    int nno, nsd;
    int nel, ndofs;
    double *coords;
    int *connect;
    Cell *cell;
};


// ---------------------------------------------------------------------------

#endif
