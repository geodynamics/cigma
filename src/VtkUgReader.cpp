#include "VtkUgReader.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"
#include <cassert>


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
get_connectivity(int **connectivity, int *nel, int *ndofs)
{
    assert(reader != 0);

    vtkCellArray *cellArray;
    vtkIdType numCells;
    vtkIdType *cellArrayPtr;

    cellArray = grid->GetCells();
    numCells = grid->GetNumberOfCells();
    cellArrayPtr = cellArray->GetPointer();

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
get_coordinates(double **coordinates, int *nno, int *nsd)
{
    assert(reader != 0);

    vtkPoints *points;
    vtkPointData *pointData;
    int pointDataDims[2];
    vtkDoubleArray *vectors;
    int vectorDims[2];

    points = grid->GetPoints();
    pointData = grid->GetPointData();
    pointDataDims[0] = pointData->GetNumberOfTuples();
    pointDataDims[1] = pointData->GetNumberOfComponents();

    vectors = static_cast<vtkDoubleArray*>(pointData->GetVectors());
    vectorDims[0] = vectors->GetNumberOfTuples();
    vectorDims[1] = vectors->GetNumberOfComponents();

    double *v = vectors->GetPointer(0);
    double *coords = new double[vectorDims[0] * vectorDims[1]];

    for (int n = 0; n < vectorDims[0]; ++n)
    {
        int offset = vectorDims[1] * n;
        for (int i = 0; i < vectorDims[1]; ++i)
        {
            coords[offset + i] = v[offset + i];
        }
    }

    *coordinates = coords;
    *nno = vectorDims[0];
    *nsd = vectorDims[1];
}

void cigma::VtkUgReader::
get_vector_point_data(const char *name, double **vectors, int *num, int *dim)
{
    assert(reader != 0);

    vtkPointData *pointData;
    vtkDataArray *dataArray;
    vtkDoubleArray *vectorsArray;
    double *v;

    pointData = grid->GetPointData();

    dataArray = 0;
    if (name != 0)
        dataArray = pointData->GetVectors(name);
    else
        dataArray = pointData->GetVectors();
    assert(dataArray != 0);

    // XXX: how to tell type of array?
    vectorsArray = static_cast<vtkDoubleArray*>(dataArray);
    //vectorsArray->PrintSelf(std::cout, 0);

    v = vectorsArray->GetPointer(0);
    // XXX: copy?

    *vectors = v;
    *num = vectorsArray->GetNumberOfTuples();
    *dim = vectorsArray->GetNumberOfComponents();
}

void cigma::VtkUgReader::
get_scalar_point_data(const char *name, double **scalars, int *num, int *dim)
{
    assert(reader != 0);

    vtkPointData *pointData;
    vtkDataArray *dataArray;
    vtkDoubleArray *scalarsArray;
    double *s;

    pointData = grid->GetPointData();

    dataArray = 0;
    if (name != 0)
        dataArray = pointData->GetScalars(name);
    else
        dataArray = pointData->GetScalars();
    assert(dataArray != 0);

    // XXX: how to tell type of array?
    scalarsArray = static_cast<vtkDoubleArray*>(dataArray);
    //scalarsArray->PrintSelf(std::cout, 0);

    s = scalarsArray->GetPointer(0);

    *scalars = s;
    *num = scalarsArray->GetNumberOfTuples();
    *dim = scalarsArray->GetNumberOfComponents();
}

// ---------------------------------------------------------------------------
