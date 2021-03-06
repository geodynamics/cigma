#ifndef __HDF_WRITER_H__
#define __HDF_WRITER_H__

#include "Writer.h"
#include "hdf5.h"


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
    int open(std::string filename);
    void close();

public:
    int write_coordinates(const char *loc, double *coordinates, int nno, int nsd);
    int write_connectivity(const char *loc, int *connectivity, int nel, int ndofs);

public:
    void write_field(FE_Field *field);

public:
    hid_t file_id;
};


#endif
