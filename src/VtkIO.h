#ifndef __VTKIO_H__
#define __VTKIO_H__

#include "DataIO.h"
#include <cstdio>

namespace cigma
{
    class VtkIO;
}


class cigma::VtkIO : public cigma::DataIO
{
public:
    VtkIO();
    ~VtkIO();

public:

public:
    bool read_connectivity(int **connect, int *nel, int *ndofs);
    bool read_coordinates(double **coords, int *nno, int *nsd);
    bool read_dofs(double **dofs, int *nno, int *ndim);
    bool read_points(double **points, int *npts, int *ndim);

public:
    void write_header();
    void write_points(double *points, int npts, int ndim);
    void write_cells(int *cells, int nel, int ndofs);
    void write_cell_types(int nsd, int nel, int ndofs);
    void write_point_data(const char *name, double *data, int nno, int ndim);
    void write_cell_data(const char *name, double *data, int nel, int ndim);

private:
    FILE *fp;
};

#endif
