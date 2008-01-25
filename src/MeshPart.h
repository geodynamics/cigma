#ifndef __MESH_PART_H__
#define __MESH_PART_H__

#include "Cell.h"
#include "Locator.h"

namespace cigma
{
    class MeshPart;
}

class cigma::MeshPart
{
public:
    MeshPart();
    ~MeshPart();

public:
    void set_coordinates(double *coordinates, int nno, int nsd);
    void set_connectivity(int *connectivity, int nel, int ndofs);
    void set_locator(Locator *locator);
    void set_cell();

public:
    void select_cell(int e);

public:
    void get_bbox(double *minpt, double *maxpt);
    void get_cell_coords(int cellIndex, double *globalCoords);
    bool find_cell(double *globalPoint, int *cellIndex);

public:
    int nno, nsd;
    int nel, ndofs;
    double *coords;
    int *connect;

public:
    Cell *cell;
    Locator *locator;

};


// ---------------------------------------------------------------------------

#endif
