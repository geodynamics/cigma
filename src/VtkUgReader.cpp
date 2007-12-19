#include <iostream>
#include <cstdlib>
#include <cassert>

#include "VtkUgReader.h"

#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"


// ---------------------------------------------------------------------------

cigma::VtkUgReader::VtkUgReader()
{
    reader = 0;
    grid = 0;
}

cigma::VtkUgReader::~VtkUgReader()
{
    reader->Delete();
}


// ---------------------------------------------------------------------------

void cigma::VtkUgReader::open(std::string filename)
{
    reader = vtkUnstructuredGridReader::New();
    reader->SetFileName(filename.c_str());
    reader->Update();
    //reader->PrintSelf(std::cout, 0);

    grid = reader->GetOutput();
    //grid->PrintSelf(std::cout, 4);
}


// ---------------------------------------------------------------------------

void cigma::VtkUgReader::
get_coordinates(double **coordinates, int *nno, int *nsd)
{
    assert(grid != 0);

    vtkPoints *points = grid->GetPoints();
    //points->PrintSelf(std::cout, 0);

    int dims[2];
    dims[0] = points->GetNumberOfPoints();
    dims[1] = 3;

    int size = dims[0] * dims[1];
    int dataType = points->GetDataType();
    double *coords = new double[size];

    if (dataType == VTK_DOUBLE)
    {
        double *ptr = static_cast<double*>(points->GetVoidPointer(0));
        for (int i = 0; i < size; i++)
        {
            coords[i] = ptr[i];
        }
    }
    else if (dataType == VTK_FLOAT)
    {
        float *ptr = static_cast<float*>(points->GetVoidPointer(0));
        for (int i = 0; i < size; i++)
        {
            coords[i] = ptr[i];
        }
    }
    else
    {
        assert(false);
    }

    *coordinates = coords;
    *nno = dims[0];
    *nsd = dims[1];
}

void cigma::VtkUgReader::
get_connectivity(int **connectivity, int *nel, int *ndofs)
{
    assert(grid != 0);

    vtkCellArray *cellArray = grid->GetCells();
    //cellArray->PrintSelf(std::cout, 0);

    vtkIdType numCells = grid->GetNumberOfCells();
    vtkIdType *cellArrayPtr = cellArray->GetPointer();

    int dofs_per_elt = cellArrayPtr[0];
    int offset = dofs_per_elt + 1;
    int *connect = new int[numCells * dofs_per_elt];
    
    for (int e = 0; e < numCells; ++e)
    {
        for (int i = 1; i <= dofs_per_elt; ++i)
        {
            connect[dofs_per_elt * e + (i-1)] = cellArrayPtr[offset*e + i];
        }
    }

    *connectivity = connect;
    *nel = numCells;
    *ndofs = dofs_per_elt;
}


void cigma::VtkUgReader::
get_vector_point_data(const char *name, double **vectors, int *num, int *dim)
{
    assert(grid != 0);

    vtkPointData *pointData = grid->GetPointData();
    //pointData->PrintSelf(std::cout, 0);

    vtkDataArray *dataArray;
    if (name != 0) {
        assert(pointData->HasArray(name) == 1);
        dataArray = pointData->GetVectors(name);
    } else {
        dataArray = pointData->GetScalars();
    }
    //dataArray->PrintSelf(std::cout, 0);

    int dataType = dataArray->GetDataType();
    assert(dataType == VTK_DOUBLE);
    double *ptr = static_cast<double*>(dataArray->GetVoidPointer(0));

    // XXX: copy the data, or keep the reference?
    // if dataType from the file is float, then we'd need to copy anyway
    // perhaps we need a void pointer and a type
    // new function signature would be
    //  void get_vector_point_data(const char *name, void **vectors, int *num, int *dim, int *type)

    *vectors = ptr;
    *num = dataArray->GetNumberOfTuples();
    *dim = dataArray->GetNumberOfComponents();
}

void cigma::VtkUgReader::
get_scalar_point_data(const char *name, double **scalars, int *num, int *dim)
{
    assert(grid != 0);

    vtkPointData *pointData = grid->GetPointData();
    //pointData->PrintSelf(std::cout, 0);

    vtkDataArray *dataArray;
    if (name != 0) {
        assert(pointData->HasArray(name) == 1);
        dataArray = pointData->GetScalars(name);
    } else {
        dataArray = pointData->GetScalars();
    }
    //dataArray->PrintSelf(std::cout, 0);


    int dataType = dataArray->GetDataType();
    assert(dataType == VTK_DOUBLE);
    double *ptr = static_cast<double*>(dataArray->GetVoidPointer(0));

    // XXX: see comment in get_vector_point_data()

    *scalars = ptr;
    *num = dataArray->GetNumberOfTuples();
    *dim = dataArray->GetNumberOfComponents();
}

// ---------------------------------------------------------------------------
