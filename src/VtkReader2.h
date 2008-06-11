#ifndef __VTK_READER2_H__
#define __VTK_READER2_H__

#include "Reader.h"

#include "vtkDataSetReader.h"
#include "vtkXMLDataReader.h"

#include "vtkUnstructuredGridReader.h"
#include "vtkStructuredGridReader.h"
#include "vtkRectilinearGridReader.h"

#include "vtkXMLUnstructuredGridReader.h"
#include "vtkXMLStructuredGridReader.h"
#include "vtkXMLRectilinearGridReader.h"

#include "vtkXMLPUnstructuredGridReader.h"
#include "vtkXMLPStructuredGridReader.h"
#include "vtkXMLPRectilinearGridReader.h"


namespace cigma
{
    class VtkReader;
}


class cigma::VtkReader : public Reader
{
public:
    typedef enum {
        NULL_GRID_READER, VTK, VTU, VTS, VTR, PVTU, PVTS, PVTR
    } GridReaderType;

public:
    VtkReader();
    ~VtkReader();

public:
    ReaderType getType() { return VTK_READER; }

public:
    int open(const char *filename);
    int close();

public:
    int get_dataset(const char *loc, double **data, int *num, int *dim);
    int get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd);
    int get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs);

public:
    GridReaderType gridReaderType;
    vtkDataSetReader *legacy_reader;
    vtkXMLReader *xml_reader;

public:
    vtkDataSet *dataset;

};


#endif
