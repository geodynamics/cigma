#ifndef __MESH_PART_H__
#define __MESH_PART_H__

#include <vector>

namespace cigma
{
    class Cell;
    class MeshPart;
}



/*
 * MeshPart - Container for geometric/topological information on section of mesh
 */
class cigma::MeshPart
{
public:

    Cell *cell;
    
    int nno;
    int nsd;
    double *coords;

    int nel;
    int ndofs;
    int *connect;

public:
    MeshPart();
    MeshPart(Cell *cell);
    ~MeshPart();

public:
    void set_cell(Cell *cell);
    void set_coordinates(double *coords, int nno, int nsd);
    void set_connectivity(int *connect, int nel, int ndofs);

public:
    void load_cell_coords(int e, double *nodes);

public:
    bool find_cell0(double *point, int& e);
    bool find_cell(double *point, int& e);
    void find_cells(int nodeIdx, std::vector<int> &eltIndices);
};

//----------------------------------------------------------------------------


#endif
