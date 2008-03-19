#ifndef __VTK_WRITER_H__
#define __VTK_WRITER_H__

#include <cstdio>
#include "Writer.h"


namespace cigma
{
    class VtkWriter;
}


class cigma::VtkWriter : public Writer
{
public:
    VtkWriter();
    ~VtkWriter();

public:
    WriterType getType() { return VTK_WRITER; }

public:
    int open(const char *filename);
    int close();

public:
    void write_header();
    void write_points(double *points, int npts, int ndim);
    void write_cells(int *cells, int nel, int ndofs);
    void write_cell_types(int nsd, int nel, int ndofs);
    void write_point_data(const char *name, double *data, int nno, int ndim);
    void write_cell_data(const char *name, double *data, int nel, int ndim);

public:
    FILE *fp;
};


#endif
