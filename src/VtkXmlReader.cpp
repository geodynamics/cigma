#include <iostream>
#include <cassert>
#include "VtkXmlReader.h"

#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"


using namespace std;


// ---------------------------------------------------------------------------

cigma::VtkXmlReader::VtkXmlReader()
{
    reader = 0;
    grid = 0;
}

cigma::VtkXmlReader::~VtkXmlReader()
{
    //close();  // XXX: make sure this call resets both reader and grid
}

// ---------------------------------------------------------------------------

int cigma::VtkXmlReader::
open(std::string filename)
{
    reader = vtkXMLStructuredGridReader::New();
    reader->SetFileName(filename.c_str());
    reader->Update();
    //cout << "Reading " << filename << endl;
    //reader->PrintSelf(cout, 4)

    grid = reader->GetOutput();
    //cout << endl << endl;
    //grid->PrintSelf(cout, 4);

    return 0;
}

void cigma::VtkXmlReader::
close()
{
    // XXX: don't forget to reset values of reader and grid
}


// ---------------------------------------------------------------------------

void cigma::VtkXmlReader::
get_dataset(const char *loc, double **data, int *num, int *dim)
{
    assert(grid != 0);
    assert(loc != 0);

    int size = 0;
    int dims[2] = {0, 0};
    double *array = 0;

    vtkPointData *pointData = grid->GetPointData();

    if (pointData->HasArray(loc) == 1)
    {
        vtkDataArray *dataArray = pointData->GetArray(loc);
        assert(dataArray != 0);

        dims[0] = dataArray->GetNumberOfTuples();
        dims[1] = dataArray->GetNumberOfComponents();

        size = dims[0] * dims[1];
        array = new double[size];

        int dataType = dataArray->GetDataType();

        if (dataType == VTK_DOUBLE)
        {
            double *ptr = static_cast<double*>(dataArray->GetVoidPointer(0));
            for (int i = 0; i < size; i++)
            {
                array[i] = ptr[i];
            }
        }
        else if (dataType == VTK_FLOAT)
        {
            float *ptr = static_cast<float*>(dataArray->GetVoidPointer(0));
            for (int i = 0; i < size; i++)
            {
                array[i] = ptr[i];
            }
        }
        else
        {
            assert(false); // XXX: throw exception instead
        }
    }

    *data = array;
    *num = dims[0];
    *dim = dims[1];
}

void cigma::VtkXmlReader::
get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd)
{
    get_coordinates(coordinates, nno, nsd);
}

void cigma::VtkXmlReader::
get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs)
{
    get_connectivity(connectivity, nel, ndofs);
}


// ---------------------------------------------------------------------------

void cigma::VtkXmlReader::
get_coordinates(double **coordinates, int *nno, int *nsd)
{
    assert(grid != 0);

    vtkPoints *points = grid->GetPoints();
    //points->PrintSelf(cout, 0);

    int dims[2];
    dims[0] = points->GetNumberOfPoints();
    dims[1] = 3;


    // XXX: need to know if we're loading 2D cells, so we can
    // recalculate size & nsd....if 3D, we leave things alone

    // XXX: for now, assume 2D grid of quadrangles


    //int size = dims[0] * dims[1];
    int size = dims[0] * 2;
    double *coords = new double[size];


    int dataType = points->GetDataType();

    if (dataType == VTK_DOUBLE)
    {
        double *ptr = static_cast<double*>(points->GetVoidPointer(0));
        for (int i = 0; i < dims[0]; i++)
        {
            coords[2*i + 0] = ptr[3*i + 0];
            coords[2*i + 1] = ptr[3*i + 1];
        } /*
        for (int i = 0; i < size; i++)
        {
            coords[i] = ptr[i];
        } // */
    }
    else if (dataType == VTK_FLOAT)
    {
        float *ptr = static_cast<float*>(points->GetVoidPointer(0));
        for (int i = 0; i < dims[0]; i++)
        {
            coords[2*i + 0] = ptr[3*i + 0];
            coords[2*i + 1] = ptr[3*i + 1];
        } /*
        for (int i = 0; i < size; i++)
        {
            coords[i] = ptr[i];
        } // */
    }
    else
    {
        assert(false); // XXX: throw exception instead
    }

    *coordinates = coords;
    *nno = dims[0];
    *nsd = dims[1];
}

void cigma::VtkXmlReader::
get_connectivity(int **connectivity, int *nel, int *ndofs)
{
    // 
    // XXX: for now, assume 2D grid of quadrangles
    //
    int nx, ny;
    int ex, ey;
    int *connect = 0;
    int ndofs_per_cell;
    int num_cells;
    int extent[6];

    num_cells = grid->GetNumberOfCells();
    ndofs_per_cell = 4;

    grid->GetExtent(extent);

    nx = extent[1] + 1;
    ny = extent[3] + 1;

    ex = nx - 1;
    ey = ny - 1;

    connect = new int[ex * ey * ndofs_per_cell];
    for (int j = 0; j < ny-1; j++)
    {
        for (int i = 0; i < nx-1; i++)
        {
            int e = i + j*ex;
            connect[4*e + 0] =  i    +  j*nx;
            connect[4*e + 1] = (i+1) +  j*nx;
            connect[4*e + 2] = (i+1) + (j+1)*nx;
            connect[4*e + 3] =  i    + (j+1)*nx;
        }
    }

    *connectivity = connect;
    *nel = num_cells;
    *ndofs = ndofs_per_cell;
}

// ---------------------------------------------------------------------------
