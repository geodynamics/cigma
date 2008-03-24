#include "VtkWriter.h"
#include <cassert>
#include <cstdlib>

using namespace cigma;


// ---------------------------------------------------------------------------

VtkWriter::VtkWriter()
{
    fp = NULL;
}

VtkWriter::~VtkWriter()
{
    close();
}


// ---------------------------------------------------------------------------

int VtkWriter::open(const char *filename)
{
    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        return -1;
    }

    this->write_header();

    return 0;
}

int VtkWriter::close()
{
    if (fp != NULL)
    {
        fclose(fp);
        fp = NULL;
    }
    return 0;
}


// ---------------------------------------------------------------------------

void VtkWriter::write_header()
{
    assert(fp != NULL);
    fprintf(fp, "# vtk DataFile Version 3.0\n");
    fprintf(fp, "This line is a comment\n");
    fprintf(fp, "ASCII\n");
    fprintf(fp, "DATASET UNSTRUCTURED_GRID\n");
}


void VtkWriter::write_points(double *points, int npts, int ndim)
{
    assert(fp != NULL);
    assert(ndim > 1);

    fprintf(fp, "POINTS %d double\n", npts);
    for (int i = 0; i < npts; i++)
    {
        fprintf(fp, " %g", points[ndim*i + 0]);
        fprintf(fp, " %g", points[ndim*i + 1]);
        if (ndim == 3)
            fprintf(fp, " %g", points[ndim*i + 2]);
        else
            fprintf(fp, " 0.0");
        fprintf(fp, "\n");
    }
}

void VtkWriter::write_cells(int *cells, int nel, int ndofs)
{
    assert(fp != NULL);

    fprintf(fp, "CELLS %d %d\n", nel, nel*(1 + ndofs));
    for (int i = 0; i < nel; i++)
    {
        fprintf(fp, " %d", ndofs);
        for (int j = 0; j < ndofs; j++)
        {
            fprintf(fp, " %d", cells[ndofs*i + j]);
        }
        fprintf(fp, "\n");
    }
}


void VtkWriter::write_cell_types(int nsd, int nel, int ndofs)
{
    assert(fp != NULL);

    fprintf(fp, "CELL_TYPES %d\n", nel);

    int vtkType = 0;
    if (nsd == 3)
    {
        switch (ndofs)
        {
        case  4: vtkType = 10; break; // VTK_TETRA=10
        case  8: vtkType = 12; break; // VTK_HEXAHEDRON=12
        case  6: vtkType = 13; break; // VTK_WEDGE=13
        case  5: vtkType = 14; break; // VTK_PYRAMID=14
        case 10: vtkType = 24; break; // VTK_QUADRATIC_TETRA=24
        case 20: vtkType = 25; break; // VTK_QUADRATIC_HEXAHEDRON=25
        }
    }
    else if (nsd == 2)
    {
        switch (ndofs)
        {
        case 3: vtkType =  5; break; // VTK_TRIANGLE=5
        case 4: vtkType =  9; break; // VTK_QUAD=9
        case 6: vtkType = 22; break; // VTK_QUADRATIC_TRIANGLE=22
        case 8: vtkType = 23; break; // VTK_QUADRATIC_QUAD=23
        }
    }
    assert(vtkType > 0);

    for (int i = 0; i < nel; i++)
    {
        fprintf(fp, "%d\n", vtkType);
    }
}

void VtkWriter::write_point_data(const char *name, double *data, int nno, int ndim)
{
    assert(fp != NULL);

    fprintf(fp, "POINT_DATA %d\n", nno);

    if (ndim == 1)
    {
        fprintf(fp, "SCALARS %s double 1\n", name);
        fprintf(fp, "LOOKUP_TABLE default\n");
        for (int i = 0; i < nno; i++)
        {
            fprintf(fp, "%g\n", data[i]);
        }
        return;
    }
    else if ((ndim == 2) || (ndim == 3))
    {
        fprintf(fp, "VECTORS %s double\n", name);
        for (int i = 0; i < nno; i++)
        {
            fprintf(fp, " %g", data[ndim*i + 0]);
            fprintf(fp, " %g", data[ndim*i + 1]);
            if (ndim == 3)
                fprintf(fp, " %g\n", data[ndim*i + 2]);
            else
                fprintf(fp, " 0.0\n");
        }
    }
    else if (ndim == 9)
    {
        fprintf(fp, "TENSORS %s double\n", name);
        for (int i = 0; i < nno; i++)
        {
            for (int j = 0; j < ndim; j++)
            {
                fprintf(fp, " %g", data[ndim*i + j]);
            }
            fprintf(fp, "\n");
        }
    }
}

void VtkWriter::write_cell_data(const char *name, double *data, int nel, int ndim)
{
    assert(fp != NULL);

    fprintf(fp, "CELL_DATA %d\n", nel);

    if (ndim == 1)
    {
        fprintf(fp, "SCALARS %s float 1\n", name);
        fprintf(fp, "LOOKUP_TABLE default\n");
        for (int i = 0; i < nel; i++)
            fprintf(fp, "%g\n", data[i]);
    }
    else if ((ndim == 2) || (ndim == 3))
    {
        fprintf(fp, "VECTORS %s float\n", name);
        for (int i = 0; i < nel; i++)
        {
            fprintf(fp, " %g", data[ndim*i + 0]);
            fprintf(fp, " %g", data[ndim*i + 1]);
            if (ndim == 3)
                fprintf(fp, " %g\n", data[ndim*i + 2]);
            else
                fprintf(fp, " 0.0\n");
        }
    }
    else if (ndim == 9)
    {
        fprintf(fp, "TENSORS %s float\n", name);
        for (int i = 0; i < nel; i++)
        {
            for (int j = 0; j < ndim; j++)
            {
                fprintf(fp, " %g", data[ndim*i + j]);
            }
            fprintf(fp, "\n");
        }
    }
}

// ---------------------------------------------------------------------------

int VtkWriter::write_coordinates(const char *loc, double *data, int nno, int ndim)
{
    this->write_points(data, nno, ndim);
}

int VtkWriter::write_dataset(const char *loc, double *data, int nno, int ndim)
{
    this->write_point_data(loc, data, nno, ndim);
}

// ---------------------------------------------------------------------------
