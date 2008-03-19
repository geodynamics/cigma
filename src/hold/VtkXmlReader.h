#ifndef __VTK_XML_READER_H__
#define __VTK_XML_READER_H__

#include <string>
#include "Reader.h"

#include "vtkXMLStructuredGridReader.h"
#include "vtkStructuredGrid.h"

namespace cigma
{
    class VtkXmlReader;
}

class cigma::VtkXmlReader : public Reader
{
public:
    typedef enum {
        VTK_FILE_NONE,
        VTK_FILE_STRUCTURED_GRID
        // XXX: others...
    } VtkXmlFileType;

    ReaderType getType()
    {
        return VTK_READER;
    }

    VtkXmlFileType getFileType()
    {
        if (reader != 0)
        {
            // XXX: assume structured grid for now
            return VTK_FILE_STRUCTURED_GRID;
        }
        return VTK_FILE_NONE;
    }

public:
    VtkXmlReader();
    ~VtkXmlReader();

public:
    int open(std::string filename);
    void close();

public:
    void get_dataset(const char *loc, double **data, int *num, int *dim);
    void get_coordinates(const char *log, double **coordinates, int *nno, int *nsd);
    void get_connectivity(const char *log, int **connectivity, int *nel, int *ndofs);


public:
    void get_coordinates(double **coordinates, int *nno, int *nsd);
    void get_connectivity(int **connectivity, int *nel, int *ndofs);
    void get_vector_point_data(const char *name, double **vectors, int *num, int *dim);
    void get_scalar_point_data(const char *name, double **scalars, int *num, int *dim);

public:
    vtkXMLStructuredGridReader *reader;
    vtkStructuredGrid *grid;
};

#endif
