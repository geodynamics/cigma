#ifndef __VTK_READER_H__
#define __VTK_READER_H__

#include <string> 

#include "Reader.h"

#include "vtkDataSetReader.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkUnstructuredGrid.h"

namespace cigma
{
    class VtkReader;
}

class cigma::VtkReader : public cigma::Reader
{
public:
    typedef enum {
        VTK_FILE_NONE,
        VTK_FILE_POLYDATA,
        VTK_FILE_STRUCTURED_POINTS,
        VTK_FILE_STRUCTURED_GRID,
        VTK_FILE_UNSTRUCTURED_GRID,
        VTK_FILE_RECTILINEAR_GRID
    } VtkFileType;

    ReaderType getType()
    {
        return VTK_READER;
    }

    VtkFileType getFileType()
    {
        if (reader != 0)
        {
            // XXX: are these mutually exclusive?
            if (reader->IsFilePolyData())
                return VTK_FILE_POLYDATA;
            if (reader->IsFileStructuredPoints())
                return VTK_FILE_STRUCTURED_POINTS;
            if (reader->IsFileStructuredGrid())
                return VTK_FILE_STRUCTURED_GRID;
            if (reader->IsFileUnstructuredGrid())
                return VTK_FILE_UNSTRUCTURED_GRID;
            if (reader->IsFileRectilinearGrid())
                return VTK_FILE_RECTILINEAR_GRID;
        }
        return VTK_FILE_NONE;
    }

public:
    VtkReader();
    ~VtkReader();

public:
    int open(std::string filename);
    void close();

public:
    void get_dataset(const char *loc, double **data, int *num, int *dim);
    void get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd);
    void get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs);

public:
    void get_coordinates(double **coordinates, int *nno, int *nsd);
    void get_connectivity(int **connectivity, int *nel, int *ndofs);
    void get_vector_point_data(const char *name, double **vectors, int *num, int *dim);
    void get_scalar_point_data(const char *name, double **scalars, int *num, int *dim);

public:
    vtkUnstructuredGridReader *reader;
    //vtkDataSetReader *reader;
    vtkUnstructuredGrid *grid;
};

// ---------------------------------------------------------------------------

#endif
