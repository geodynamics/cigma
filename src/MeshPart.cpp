#include <cassert>
#include "MeshPart.h"
#include "Numeric.h"

#include "Tet.h"
#include "Hex.h"
#include "Tri.h"
#include "Quad.h"

using namespace cigma;

// ---------------------------------------------------------------------------

MeshPart::MeshPart()
{
    nno = 0;
    nsd = 0;
    coords = 0;

    nel = 0;
    ndofs = 0;
    connect = 0;
    
    cell = 0;
    locator = 0;
}


MeshPart::~MeshPart()
{
    // XXX: don't delete if we copied the pointers
    if (coords) delete [] coords;
    if (connect) delete [] connect;
}


// ---------------------------------------------------------------------------

void MeshPart::set_coordinates(double *coordinates, int nno, int nsd)
{
    assert(nno > 0);
    assert(nsd > 0);

    this->nno = nno;
    this->nsd = nsd;

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


void MeshPart::set_connectivity(int *connectivity, int nel, int ndofs)
{
    assert(nel > 0);
    assert(ndofs > 0);

    this->nel = nel;
    this->ndofs = ndofs;

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

void MeshPart::set_locator(Locator *locator)
{
    this->locator = locator;
    locator->initialize(this);
}


void MeshPart::set_cell()
{
    assert(nsd > 0);
    assert(ndofs > 0);

    cell = 0;
    switch (nsd)
    {
    case 3:
        switch (ndofs)
        {
        case 4:
            cell = new Tet();
            break;
        case 8:
            cell = new Hex();
            break;
        }
        break;
    case 2:
        switch (ndofs)
        {
        case 3:
            cell = new Tri();
            break;
        case 4:
            cell = new Quad();
            break;
        }
        break;
    }
    assert(cell != 0);
}


// ---------------------------------------------------------------------------

void MeshPart::get_bbox(double *minpt, double *maxpt)
{
    cigma::minmax(coords, nno, nsd, minpt, maxpt);
}


void MeshPart::get_cell_coords(int cellIndex, double *globalCellCoords)
{
    assert(0 <= cellIndex);
    assert(cellIndex < nel);

    //
    // Note that the connectivity array is assumed
    // to be zero-based.
    //

    int i,j;
    int *conn = &connect[ndofs * cellIndex];
    for (i = 0; i < ndofs; i++)
    {
        double *pt = &coords[nsd * conn[i]];
        for (j = 0; j < 3; j++)
        {
            globalCellCoords[3*i + j] = 0.0;
        }
        for (j = 0; j < nsd; j++)
        {
            globalCellCoords[3*i + j] = pt[j];
        }
    }
}


void MeshPart::select_cell(int e)
{
    get_cell_coords(e, cell->globverts);
}


bool MeshPart::find_cell(double globalPoint[3], int *cellIndex)
{
    int i;
    int e;
    
    *cellIndex = -1;

    if (locator != 0)
    {
        locator->search_bbox(globalPoint);

        for (i = 0; i < locator->n_idx(); i++)
        {
            e = locator->idx(i);

            select_cell(e);

            if (cell->global_interior(globalPoint))
            {
                *cellIndex = e;
                return true;
            }
        }

        // XXX: give up here? 
        //return false;
    }


    /* Check every cell for given point. Since
     * quadrature points are clustered together in
     * the same cell, remember the last cell
     * that was checked and look there first.
     */
    static int last_cell = -1;

    if ((0 <= last_cell) && (last_cell < nel))
    {
        select_cell(last_cell);

        if (cell->global_interior(globalPoint))
        {
            *cellIndex = last_cell;
            return true;
        }
    } // */

    for (e = 0; e < nel; e++)
    {
        select_cell(e);

        if (cell->global_interior(globalPoint))
        {
            *cellIndex = e;
            last_cell = e;
            return true;
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
