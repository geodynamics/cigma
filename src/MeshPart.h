#ifndef __MESH_PART_H__
#define __MESH_PART_H__

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
    void set_coordinates(double *coordinates, int nno, int nsd);
    void set_connectivity(int *connectivity, int nel, int ndofs);

public:
    void cell_coords(int cellIndex, double *globalCoords);
    virtual bool find_cell(double globalPoint[3], int *cellIndex) = 0;

public:

    int nno, nsd;
    double *coords;

    int nel, ndofs;
    int *connect;

};

#endif
