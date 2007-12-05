#ifndef __TEXTREADER_H__
#define __TEXTREADER_H__

#include "DataIO.h"
#include <cstdio>

namespace cigma
{
    class TextIO;
}

class cigma::TextIO : public cigma::DataIO
{
public:
    TextIO();
    ~TextIO();

public:
    void open_read(std::string filename);
    void open_write(std::string filename);
    void close();

public:
    bool read_connectivity(int **connect, int *nel, int *ndofs);
    bool read_coordinates(double **coords, int *nno, int *nsd);
    bool read_dofs(double **dofs, int *nno, int *ndim);
    bool read_points(double **points, int *npts, int *ndim);

public:
    void write_header();
    void write_points(double *points, int npts, int ndim);
    void write_cells(int *cells, int nel, int ndofs);

private:
    FILE *fp;
};

//----------------------------------------------------------------------------


#endif
