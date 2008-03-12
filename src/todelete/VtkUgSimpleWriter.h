#ifndef __VTK_UG_SIMPLE_WRITER_H__
#define __VTK_UG_SIMPLE_WRITER_H__

#include <cstdio>
#include <string>

namespace cigma
{
    class VtkUgSimpleWriter;
}

class cigma::VtkUgSimpleWriter
{
public:
    VtkUgSimpleWriter();
    ~VtkUgSimpleWriter();

public:
    void open(std::string filename);
    void close();

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
