#ifndef __VTK_READER_H__
#define __VTK_READER_H__

#include "Reader.h"

#include "vtkDataSetReader.h"
#include "vtkXMLStructuredGridReader.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkXMLPStructuredGridReader.h"
#include "vtkXMLPUnstructuredGridReader.h"

#include "vtkDataSet.h"
#include "vtkPointSet.h"
#include "vtkStructuredGrid.h"
#include "vtkUnstructuredGrid.h"



namespace cigma
{
    class VtkReader;
}

class cigma::VtkReader : public Reader
{

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
    vtkDataSet *dataset;
    vtkPointSet *pointset;
    vtkUnstructuredGrid *ugrid;
    vtkStructuredGrid *sgrid;

public:
    vtkDataSetReader *vtk_reader;
    vtkXMLStructuredGridReader *vts_reader;
    vtkXMLUnstructuredGridReader *vtu_reader;
    vtkXMLPStructuredGridReader *pvts_reader;
    vtkXMLPUnstructuredGridReader *pvtu_reader;

};


#endif
