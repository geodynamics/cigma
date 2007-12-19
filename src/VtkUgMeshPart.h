#ifndef __VTK_UG_MESH_PART_H__
#define __VTK_UG_MESH_PART_H__

#include "vtkUnstructuredGrid.h"
#include "vtkPoints.h"
#include "vtkCellType.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"

#include "vtkTetra.h"
#include "vtkHexahedron.h"

namespace cigma
{
    class VtkUgMeshPart;
}

class cigma::VtkUgMeshPart
{
public:
    VtkUgMeshPart();
    ~VtkUgMeshPart();

public:
    void initialize();
    void set_coordinates(double *coordinates, int nno, int nsd);
    void set_connectivity(int *connectivity, int nel, int ndofs);

public:
    bool find_cell(double globalPoint[3], int *cellIndex);

public:
    int nno, nsd;
    int nel, ndofs;
    vtkUnstructuredGrid *grid;
    vtkPoints *points;
    vtkDoubleArray *coordsArray;
    vtkCellArray *cellArray;
};


#endif
