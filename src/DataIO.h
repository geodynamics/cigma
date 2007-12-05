#ifndef __DATAIO_H__
#define __DATAIO_H__

#include <string>

namespace cigma
{
    class DataIO;
}

class cigma::DataIO
{
public:
    DataIO() {}
    virtual ~DataIO();

public:
    virtual void open_read(std::string filename);
    virtual void open_write(std::string filename);
    virtual void close();

public:
    virtual bool read_connectivity(int **connect, int *nel, int *ndofs);
    virtual bool read_coordinates(double **coords, int *nno, int *nsd);
    virtual bool read_dofs(double **dofs, int *nno, int *ndim);
    virtual bool read_points(double **points, int *npts, int *ndim);

public:
    virtual void write_header();
    virtual void write_points(double *points, int npts, int ndim);
    virtual void write_cells(int *cells, int nel, int ndofs);
};

//----------------------------------------------------------------------------


#endif
