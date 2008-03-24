#ifndef __HDF_WRITER_H__
#define __HDF_WRITER_H__

#include "Writer.h"
#include "HdfFile.h"


namespace cigma
{
    class HdfWriter;
}


class cigma::HdfWriter : public Writer
{
public:
    HdfWriter();
    ~HdfWriter();

public:
    WriterType getType() { return HDF_WRITER; }

public:
    int open(const char *filename);
    int close();

public:
    int write_dataset(const char *loc, double *data, int nno, int ndim);
    int write_coordinates(const char *loc, double *coordinates, int nno, int nsd);
    int write_connectivity(const char *loc, int *connectivity, int nel, int ndofs);

public:
    int write_int_dataset(const char *loc, int *data, int rows, int cols);

public:
    HdfFile h5;
};


#endif
