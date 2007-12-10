#include <iostream>
#include <cassert>
#include "vtkUnstructuredGridReader.h"
#include "vtkUnstructuredGrid.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"

// ---------------------------------------------------------------------------
// This class is a wrapper around vtkUnstructuredGridReader

class VtkReader
{
public:
    VtkReader();
    ~VtkReader();

public:
    void open(std::string filename);
    void get_connectivity(int **connectivity, int *nel, int *ndofs);
    void get_coordinates(double **coordinates, int *nno, int *nsd);
    void get_vector_point_data(const char *name, double **vectors, int *num, int *dim);
    void get_scalar_point_data(const char *name, double **scalars, int *num, int *dim);

public:
    vtkUnstructuredGridReader *reader;
    vtkUnstructuredGrid *grid;
};


// ---------------------------------------------------------------------------

VtkReader::VtkReader()
{
    reader = 0;
    grid = 0;
}

VtkReader::~VtkReader()
{
    reader->Delete();
}

void VtkReader::open(std::string filename)
{
    reader = vtkUnstructuredGridReader::New();
    reader->SetFileName(filename.c_str());
    reader->Update();
    //reader->PrintSelf(std::cout,0);

    grid = reader->GetOutput();
    //grid->PrintSelf(std::cout, 4);
}


void VtkReader::get_connectivity(int **connectivity, int *nel, int *ndofs)
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
            connect[dofs_per_elt * e + (i-1)] = cellArrayPtr[offset*e + i];
    }

    *connectivity = connect;
    *nel = numCells;
    *ndofs = dofs_per_elt;
}


void VtkReader::get_coordinates(double **coordinates, int *nno, int *nsd)
{
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
    double *coords = new double[vectorDims[0]*vectorDims[1]];
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


void VtkReader::get_vector_point_data(const char *name, double **vectors, int *num, int *dim)
{
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

    // XXX: how to tell type of array? (double vs. float)
    vectorsArray = static_cast<vtkDoubleArray*>(dataArray);
    //vectorsArray->PrintSelf(std::cout, 0);

    v = vectorsArray->GetPointer(0);
    
    *vectors = v;
    *num = vectorsArray->GetNumberOfTuples();
    *dim = vectorsArray->GetNumberOfComponents();
}


void VtkReader::get_scalar_point_data(const char *name, double **scalars, int *num, int *dim)
{
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

int main(void)
{
    std::string filename = "strikeslip_tet4_1000m_t0.vtk";
    VtkReader *reader = new VtkReader();

    int nel, ndofs;
    int *connect;

    int nno, nsd;
    double *coords;

    int disp_nno, disp_dim;
    double *displacement;

    int scalars_nno, scalars_dim;
    double *scalars;


    reader->open(filename);
    reader->get_connectivity(&connect, &nel, &ndofs);
    reader->get_coordinates(&coords, &nno, &nsd);

    reader->get_vector_point_data("displacements_t0", &displacement, &disp_nno, &disp_dim);
    assert(nno == disp_nno);
    assert(nsd == 3);

    reader->get_scalar_point_data(0, &scalars, &scalars_nno, &scalars_dim);
    assert(nno = scalars_nno);
    assert(scalars_dim == 4);


    std::cout << "coords :: [ " << nno << " x " << nsd << " ]\n";
    std::cout << "connect :: [ " << nel << " x " << ndofs << " ]\n";
    std::cout << "displacement :: [ " << disp_nno << " x " << disp_dim << " ]\n";
    std::cout << "scalars :: [ " << scalars_nno << " x " << scalars_dim << " ]\n";

    /*
    for (int i = 0; i < nno; i++)
    {
        for (int j = 0; j < nsd; j++)
        {
            std::cout << coords[nsd*i + j] << " ";
        }
        std::cout << std::endl;
    } // */

    /*
    for (int i = 0; i < nel; i++)
    {
        for (int j = 0; j < ndofs; j++)
        {
            std::cout << connect[ndofs*i + j] << " ";
        }
        std::cout << std::endl;
    } // */

    /*
    for (int i = 0; i < disp_nno; i++)
    {
        for (int j = 0; j < disp_dim; j++)
        {
            std::cout << displacement[disp_dim * i + j] << " ";
        }
        std::cout << std::endl;
    } // */

    /*
    for (int i = 0; i < scalars_nno; i++)
    {
        for (int j = 0; j < scalars_dim; j++)
        {
            std::cout << scalars[scalars_dim * i + j] << " ";
        }
        std::cout << std::endl;
    } // */

    delete reader;
    return 0;
}

