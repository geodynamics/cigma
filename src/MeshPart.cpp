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
    
    cell = 0;
}


cigma::MeshPart::
~MeshPart()
{
    //* // XXX: don't delete if we copied the pointers
    if (coords) delete [] coords;
    if (connect) delete [] connect;
    // */
}


// ---------------------------------------------------------------------------

void cigma::MeshPart::
set_coordinates(double *coordinates, int nno, int nsd)
{
    //assert(nno > 0);
    //assert(nsd > 0);

    this->nno = nno;
    this->nsd = nsd;

    //assert(nno == n_nodes());
    //assert(nsd == n_nsd());

    /* // XXX: copy pointer
    coords = coordinates;
    // */

    //* // XXX: copy data (use memcpy?)
    coords = new double[nno * nsd];
    for (int i = 0; i < nno; i++)
    {
        for (int j = 0; j < nsd; j++)
        {
            int n = nsd*i + j;
            coords[n] = coordinates[n];
        }
    } // */
}


void cigma::MeshPart::
set_connectivity(int *connectivity, int nel, int ndofs)
{
    //assert(nel > 0);
    //assert(ndofs > 0);

    this->nel = nel;
    this->ndofs = ndofs;

    //assert(nel == n_nel());
    //assert(ndofs == n_ndofs());

    /* // XXX: copy pointer
    connect = connectivity;
    // */

    //* // XXX: copy data (use memcpy?)
    connect = new int[nel * ndofs];
    for (int i = 0; i < nel; i++)
    {
        for (int j = 0; j < ndofs; j++)
        {
            int e = ndofs*i + j;
            connect[e] = connectivity[e];
        }
    } // */
}


// ---------------------------------------------------------------------------

void cigma::MeshPart::
get_cell_coords(int cellIndex, double *globalCellCoords)
{
    //assert(nsd > 0);
    //assert(ndofs > 0);

    //const int nel = n_nel();
    //const int ndofs = n_ndofs();
    //const int nsd = n_nsd();

    assert(0 <= cellIndex);
    assert(cellIndex < nel);

    int *conn = &connect[ndofs * cellIndex];

    for (int i = 0; i < ndofs; i++)
    {
        double *pt = &coords[nsd * conn[i]];
        for (int j = 0; j < nsd; j++)
        {
            globalCellCoords[nsd*i + j] = pt[j];
        }
    }
}

// ---------------------------------------------------------------------------
