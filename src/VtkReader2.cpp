#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include "StringUtils.h"
#include "PathUtils.h"

#include "VtkReader2.h"

#include "vtkDataSet.h"
#include "vtkPointData.h"

#include "vtkDataArray.h"
#include "vtkDataArraySelection.h"

#include "vtkIdList.h"
#include "vtkCell.h"
#include "vtkGenericCell.h"



using namespace std;
using namespace cigma;


// ----------------------------------------------------------------------------

VtkReader::VtkReader()
{
    dataset = 0;
    legacy_reader = 0;
    xml_reader = 0;
    gridReaderType = NULL_GRID_READER;
}

VtkReader::~VtkReader()
{
    close();
}

// ----------------------------------------------------------------------------

int VtkReader::open(const char *filename)
{
    //cout << ">> VtkReader::open(): Reading " << filename << endl; // XXX: need journal logging

    string filepath = filename;
    string fileroot, ext;
    path_splitext(filepath, fileroot, ext);

    if (!VtkExtension(ext.c_str()))
    {
        return -1;
    }

    int status;

    if (ext == ".vtk")
    {
        legacy_reader = vtkDataSetReader::New();

        legacy_reader->SetFileName(filename);

        status = legacy_reader->OpenVTKFile();

        if (status == 0)
        {
            //cerr << "Could not read VTK file " << filename << endl;
            return -1;
        }

        gridReaderType = VTK;

        legacy_reader->Update();

        dataset = legacy_reader->GetOutput();
        dataset->Update();

    }
    else
    {
        if (ext == ".vtu")
        {
            xml_reader = vtkXMLUnstructuredGridReader::New();
            gridReaderType = VTU;
        }
        else if (ext == ".vts")
        {
            xml_reader = vtkXMLStructuredGridReader::New();
            gridReaderType = VTS;
        }
        else if (ext == ".vtr")
        {
            xml_reader = vtkXMLRectilinearGridReader::New();
            gridReaderType = VTR;
        }
        else if (ext == ".pvtu")
        {
            xml_reader = vtkXMLPUnstructuredGridReader::New();
            gridReaderType = PVTU;
        }
        else if (ext == ".pvts")
        {
            xml_reader = vtkXMLPStructuredGridReader::New();
            gridReaderType = PVTS;
        }
        else if (ext == ".pvtr")
        {
            xml_reader = vtkXMLPRectilinearGridReader::New();
            gridReaderType = PVTR;
        }
        else
        {
            cerr << "Unsupported VTK file (" << ext << ")" << endl;
            return -1;
        }

        status = xml_reader->CanReadFile(filename);

        if (status == 0)
        {
            cerr << "Could not read XML VTK file " << filename << endl;
            gridReaderType = NULL_GRID_READER;
            return -1;
        }

        xml_reader->SetFileName(filename);
        xml_reader->Update();
        xml_reader->GetPointDataArraySelection()->EnableAllArrays();

        dataset = xml_reader->GetOutputAsDataSet();
        dataset->Update();
    }
    return 0;
}


int VtkReader::close()
{
    gridReaderType = NULL_GRID_READER;

    if (dataset != 0)
    {
        dataset = 0;
    }

    if (legacy_reader)
    {
        legacy_reader->Delete();
        legacy_reader = 0;
    }

    if (xml_reader)
    {
        xml_reader->Delete();
        xml_reader = 0;
    }

    return 0;
}



// ----------------------------------------------------------------------------

static int GetDimensionFromCellType(int cellType)
{
    if ((cellType == VTK_TRIANGLE) ||
        (cellType == VTK_QUADRATIC_TRIANGLE) ||
        (cellType == VTK_PARAMETRIC_TRI_SURFACE) ||
        (cellType == VTK_HIGHER_ORDER_TRIANGLE))
    {
        return 2;
    }

    if ((cellType == VTK_QUAD) ||
        (cellType == VTK_QUADRATIC_QUAD) ||
        (cellType == VTK_PARAMETRIC_QUAD_SURFACE) ||
        (cellType == VTK_HIGHER_ORDER_QUAD))
    {
        return 2;
    }

    if ((cellType == VTK_TETRA) ||
        (cellType == VTK_QUADRATIC_TETRA) ||
        (cellType == VTK_PARAMETRIC_TETRA_REGION) ||
        (cellType == VTK_HIGHER_ORDER_TETRAHEDRON))
    {
        return 3;
    }

    if ((cellType == VTK_VOXEL) ||
        (cellType == VTK_HEXAHEDRON) ||
        (cellType == VTK_QUADRATIC_HEXAHEDRON) ||
        (cellType == VTK_PARAMETRIC_HEX_REGION) ||
        (cellType == VTK_HIGHER_ORDER_HEXAHEDRON))
    {
        return 3;
    }

    return -1;
}

int VtkReader::get_dataset(const char *loc, double **data, int *num, int *dim)
{
    //cout << ">> VtkReader::get_dataset(): requesting location " << loc << endl;

    int i,j;
    int n_points;
    int rank;
    double *array = 0;

    if (dataset == 0)
    {
        return -1;
    }

    vtkPointData *pointData = dataset->GetPointData();

    assert(pointData != 0);

    if (pointData->HasArray(loc) == 1)
    {
        vtkDataArray *dataArray = pointData->GetArray(loc);
        assert(dataArray != 0);

        /* XXX: verify that we are loading a dataset of the correct type
        int dataType = dataArray->GetDataType();
        if ((dataType != VTK_DOUBLE) || (dataType != VTK_FLOAT))
        {
            assert(false); // XXX
            return -1;
        }*/

        n_points = dataArray->GetNumberOfTuples();
        rank = dataArray->GetNumberOfComponents();
        array = new double[n_points * rank];

        for (i = 0; i < n_points; i++)
        {
            double dofval[rank];
            dataArray->GetTuple(i, dofval);
            for (j = 0; j < rank; j++)
            {
                array[i*rank + j] = dofval[j];
            }
        }
    }
    else
    {
        assert(false); // XXX
        return -1;
    }

    *data = array;
    *num = n_points;
    *dim = rank;

    return 0;
}

int VtkReader::get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd)
{
    //cout << ">> VtkReader::get_coordinates(): requesting location " << loc << endl;

    int i,j;
    int n_points;
    int n_dims;
    double *array = 0;

    if (dataset == 0)
    {
        return -1;
    }

    int cellType = dataset->GetCellType(0); // XXX: assumes uniform cell types

    n_points = dataset->GetNumberOfPoints();
    n_dims = GetDimensionFromCellType(cellType);

    assert(n_dims <= 3);

    if (n_dims < 0)
    {
        assert(false);
        return -1;
    }

    array = new double[n_points * n_dims];

    for (i = 0; i < n_points; i++)
    {
        double point[3];
        dataset->GetPoint(i, point);
        for (j = 0; j < n_dims; j++)
        {
            array[i*n_dims + j] = point[j];
        }
    }

    *coordinates = array;
    *nno = n_points;
    *nsd = n_dims;

    return 0;
}

int VtkReader::get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs)
{
    //cout << ">> VtkReader::get_connectivity(): requesting location " << loc << endl;

    int i,j;
    int n_cells;
    int n_dofs;
    int *array = 0;

    if (dataset == 0)
    {
        return -1;
    }

    vtkCell *firstCell = dataset->GetCell(0);

    n_cells = dataset->GetNumberOfCells();
    n_dofs = firstCell->GetNumberOfPoints();    // XXX: assumes uniform cell types
    array = new int[n_cells * n_dofs];

    vtkGenericCell *cell = vtkGenericCell::New();
    for (i = 0; i < n_cells; i++)
    {
        dataset->GetCell(i, cell);
        for (j = 0; j < n_dofs; j++)
        {
            array[i*n_dofs + j] = cell->GetPointIds()->GetId(j);
        }
    }

    *connectivity = array;
    *nel = n_cells;
    *ndofs = n_dofs;

    return 0;
}


// ----------------------------------------------------------------------------
