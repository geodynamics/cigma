#include "MeshPart.h"
#include <cassert>

// ---------------------------------------------------------------------------

cigma::MeshPart::
MeshPart()
{
    nno = 0;
    nsd = 0;
    coords = 0;

    nel = 0;
    ndofs = 0;
    connect = 0;
}

cigma::MeshPart::
~MeshPart()
{
    if (coords) delete [] coords;
    if (connect) delete [] connect;
}

// ---------------------------------------------------------------------------


void cigma::MeshPart::
set_coordinates(double *coordinates, int nno, int nsd)
{
    assert(nno > 0);
    assert(nsd > 0);

    this->nno = nno;
    this->nsd = nsd;

    coords = new double[nno * nsd];

    // XXX: memcpy?
    for (int i = 0; i < nno; i++)
    {
        for (int j = 0; j < nsd; j++)
        {
            int n = nsd*i + j;
            coords[n] = coordinates[n];
        }
    }
}

void cigma::MeshPart::
set_connectivity(int *connectivity, int nel, int ndofs)
{
    assert(nel > 0);
    assert(ndofs > 0);

    this-> nel = nel;
    this-> ndofs = ndofs;

    connect = new int[nel * ndofs];

    // XXX: memcpy?
    for (int i = 0; i < nel; i++)
    {
        for (int j = 0; j < ndofs; j++)
        {
            int e = ndofs*i + j;
            connect[e] = connectivity[e];
        }
    }
}


void cigma::MeshPart::
cell_coords(int cellIndex, double *globalCellCoords)
{
    assert(nsd > 0);
    assert(ndofs > 0);

    int *conn;

    conn = &connect[ndofs * cellIndex];
    for (int i = 0; i < ndofs; i++)
    {
        double *coor = &coords[conn[i]];
        for (int j = 0; j < nsd; j++)
        {
            globalCellCoords[nsd*i + j] = coor[j];
        }
    }
}
