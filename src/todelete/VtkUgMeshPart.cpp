#include <cstdlib>
#include <cassert>
#include "VtkUgMeshPart.h"

// ---------------------------------------------------------------------------

cigma::VtkUgMeshPart::
VtkUgMeshPart()
{
    grid = vtkUnstructuredGrid::New();
    //grid->PrintSelf(std::cout, 0);

    points = 0;

    //nno = 0;
    //nsd = 0;
    coordsArray = 0;

    //nel = 0;
    //ndofs = 0;
    cellArray = 0;
}

cigma::VtkUgMeshPart::
~VtkUgMeshPart()
{
}

// ---------------------------------------------------------------------------

void cigma::VtkUgMeshPart::set_coordinates(double *coordinates, int nno, int nsd)
{
    assert(coordinates != NULL);

    this->nno = nno;
    this->nsd = nsd;

    coordsArray = vtkDoubleArray::New();
    coordsArray->SetNumberOfTuples(nno);
    coordsArray->SetNumberOfComponents(nsd);
    coordsArray->SetVoidArray(coordinates, nno*nsd, 1);
    //coordsArray->PrintSelf(std::cout, 0);

    points = vtkPoints::New();
    points->SetData(coordsArray);
    //points->SetDataTypeToDouble(); // XXX:
    //points->PrintSelf(std::cout, 0);

    grid->SetPoints(points);
    grid->GetPointData()->SetVectors(coordsArray);

}

void cigma::VtkUgMeshPart::set_connectivity(int *connectivity, int nel, int ndofs)
{
    assert(connectivity != NULL);

    this->nel = nel;
    this->ndofs = ndofs;

    cellArray = vtkCellArray::New();
    for (int e = 0; e < nel; e++)
    {
        cellArray->InsertNextCell(ndofs);
        for (int i = 0; i < ndofs; i++)
        {
            cellArray->InsertCellPoint(connectivity[ndofs*e + i]);
        }
    }
    //cellArray->PrintSelf(std::cout, 0);

    if (ndofs == 4) {
        std::cout << "VTK_TETRA" << std::endl;
        grid->SetCells(VTK_TETRA, cellArray);
    } else if (ndofs == 8) {
        std::cout << "VTK_HEXAHEDRON" << std::endl;
        grid->SetCells(VTK_HEXAHEDRON, cellArray);
    } else {
        assert(false);
    }
}
// ---------------------------------------------------------------------------

bool cigma::VtkUgMeshPart::
find_cell(double globalPoint[3], int *cellIndex)
{
    /*
    vtkTetra *cell = vtkTetra::New();

    double tol2 = 1e-6;
    double weights[4];
    double pcoords[3];
    int subId;

    //grid->FindCell(globalPoint, cell, 0, tol2, subId, pcoords, weights);

    std::cout << "ID = " << subId << std::endl;
    std::cout << "REFCOORDS = " << pcoords[0] << " " << pcoords[0] << " " << pcoords[0] << std::endl;

    // */

    //*
    const bool verbose = false;
    if (verbose)
    {
        std::cout << "Searching for cell containing (" << globalPoint[0] << ", " << globalPoint[1] << ", " << globalPoint[2] << ") ";
    }


    static int last_e = -1;
    *cellIndex = -1;

    //*
    if ((0 <= last_e) && (last_e < nel))
    {
        double uvw[3];
        get_cell_coords(last_e, cell->globverts);
        cell->xyz2uvw(globalPoint, uvw);
        if (cell->interior(uvw[0], uvw[1], uvw[2]))
        {
            if (verbose) { std::cout << last_e << std::endl; }
            *cellIndex = last_e;
            return true;
        }
    } // */

    for (int e = 0; e < nel; e++)
    {
        double uvw[3];
        get_cell_coords(e, cell->globverts);
        cell->xyz2uvw(globalPoint, uvw);
        if (cell->interior(uvw[0], uvw[1], uvw[2]))
        {
            if (verbose) { std::cout << e << std::endl; }
            *cellIndex = e;
            last_e = e;
            return true;
        }
    } // */
    if (verbose) { std::cout << " -> not found!\n"; }
    return false;

    /*
    *cellIndex = 0;
    return true;
    // */
}
