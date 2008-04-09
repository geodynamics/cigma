#include <string>
#include <cstdlib>
#include <cassert>

#include "StringUtils.h"

#include "VtkReader.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"


using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

VtkReader::VtkReader()
{
    dataset = 0;
    pointset = 0;
    ugrid = 0;
    sgrid = 0;

    vtk_reader = 0;
    vtu_reader = 0;
    vts_reader = 0;
    vtr_reader = 0;
    pvtu_reader = 0;
    pvts_reader = 0;
    pvtr_reader = 0;
}

VtkReader::~VtkReader()
{
    close();
}

// ---------------------------------------------------------------------------


int VtkReader::open(const char *filename)
{
    //cout << "Reading " << filename << endl;

    string filepath = filename;
    string fileroot, ext;
    path_splitext(filepath, fileroot, ext);

    if (ext == ".vtk")
    {
        int outputType;

        vtk_reader = vtkDataSetReader::New();
        vtk_reader->SetFileName(filename);
        vtk_reader->Update();

        outputType = vtk_reader->ReadOutputType();

        if (outputType == VTK_STRUCTURED_GRID)
        {
            sgrid = vtk_reader->GetStructuredGridOutput();
            dataset = sgrid;
            pointset = sgrid;
        }
        else if (outputType == VTK_UNSTRUCTURED_GRID)
        {
            ugrid = vtk_reader->GetUnstructuredGridOutput();
            dataset = ugrid;
            pointset = ugrid;
        }
        else
        {
            cerr << "Unsupported VTK file. "
                 << "Use only structured & unstructured grids."
                 << endl;
            return -1;
        }
    }
    else if (ext == ".vtu")
    {
        vtu_reader = vtkXMLUnstructuredGridReader::New();
        vtu_reader->SetFileName(filename);
        vtu_reader->Update();

        dataset = vtu_reader->GetOutputAsDataSet();
        pointset = vtkPointSet::SafeDownCast(dataset);
        ugrid = vtkUnstructuredGrid::SafeDownCast(dataset);
    }
    else if (ext == ".vts")
    {
        vts_reader = vtkXMLStructuredGridReader::New();
        vts_reader->SetFileName(filename);
        vts_reader->Update();

        dataset = vts_reader->GetOutputAsDataSet();
        sgrid = vtkStructuredGrid::SafeDownCast(dataset);
    }
    else if (ext == ".vtr")
    {
        vtr_reader = vtkXMLRectilinearGridReader::New();
        vtr_reader->SetFileName(filename);
        vtr_reader->Update();

        //dataset = vtr_reader->GetOutputAsDataset();
        //rgrid = vtkRectilinearGrid::SafeDownCast(dataset);
    }
    else
    {
        cerr << "Unsupported VTK file type (" << ext << ")" << endl;
        return -1;
    }

    return 0;
}

int VtkReader::close()
{
    if (this->vtk_reader)
    {
        this->vtk_reader->Delete();
        this->vtk_reader = 0;
    }
    
    if (this->vtu_reader)
    {
        this->vtu_reader->Delete();
        this->vtu_reader = 0;
    }

    if (this->vts_reader)
    {
        this->vts_reader->Delete();
        this->vts_reader = 0;
    }

    if (this->vtr_reader)
    {
        this->vtr_reader->Delete();
        this->vtr_reader = 0;
    }

    return 0;
}


// ---------------------------------------------------------------------------


int VtkReader::get_dataset(const char *loc, double **data, int *num, int *dim)
{
    int size = 0;
    int dims[2] = {0, 0};
    double *array = 0;

    if (dataset == 0)
    {
        return -1;
    }

    vtkPointData *pointData = dataset->GetPointData();

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
            assert(false); // XXX: throw own exception
            return -1;
        }
    }
    else
    {
        return -1; // no array found at loc!
    }


    *data = array;
    *num = dims[0];
    *dim = dims[1];

    return 0;

}

int VtkReader::get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd)
{
    if (ugrid != 0)
    {
        vtkPoints *points = pointset->GetPoints();
        //points->PrintSelf(cout, 0);

        int dims[2];
        dims[0] = points->GetNumberOfPoints();
        dims[1] = 3;

        int size = dims[0] * dims[1];
        int dataType = points->GetDataType();
        double *coords = 0;

        if (dataType == VTK_DOUBLE)
        {
            double *ptr;
            coords = new double[size];
            ptr = static_cast<double*>(points->GetVoidPointer(0));
            for (int i = 0; i < size; i++)
            {
                coords[i] = ptr[i];
            }
        }
        else if (dataType == VTK_FLOAT)
        {
            float *ptr;
            coords = new double[size];
            ptr = static_cast<float*>(points->GetVoidPointer(0));
            for (int i = 0; i < size; i++)
            {
                coords[i] = ptr[i];
            }
        }
        else
        {
            assert(false); // XXX: throw own exception
            return -1;
        }

        *coordinates = coords;
        *nno = dims[0];
        *nsd = dims[1];

        return 0;
    }
    else if (sgrid != 0)
    {
        vtkPoints *points = sgrid->GetPoints();
        //points->PrintSelf(cout, 0);


        // XXX: need to know if we're loading 2D cells, so we can
        // recalculate size & nsd....if 3D, we leave things alone


        int dims[2];
        dims[0] = points->GetNumberOfPoints();
        //dims[1] = 3;
        dims[1] = 2;    // XXX: for now, assume 2D grid of quadrangles

        int size = dims[0] * dims[1];
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

        return 0;
    }

    assert(false); // XXX: throw own exception
    return -1;
}


int VtkReader::get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs)
{
    if (ugrid != 0)
    {
        vtkIdType numCells;
        int dofs_per_elt;
        int *connect = 0;

        vtkCellArray *cellArray = ugrid->GetCells();
        //cellArray->PrintSelf(std::cout, 0);

        numCells = ugrid->GetNumberOfCells();

        vtkIdType *cellArrayPtr = cellArray->GetPointer();

        dofs_per_elt = cellArrayPtr[0];

        int offset = dofs_per_elt + 1;

        connect = new int[numCells * dofs_per_elt];

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

        return 0;

    }
    else if (sgrid != 0)
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

        num_cells = sgrid->GetNumberOfCells();
        ndofs_per_cell = 4;

        sgrid->GetExtent(extent);

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

        return 0;
    }

    assert(false); // XXX: throw own exception
    return -1;
}

// ---------------------------------------------------------------------------
