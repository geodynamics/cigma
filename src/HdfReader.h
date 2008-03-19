#ifndef __HDF_READER_H__
#define __HDF_READER_H__

#include "hdf5.h"
#include "Reader.h"
#include "HdfFile.h"


namespace cigma
{
    class HdfReader;
}


class cigma::HdfReader : public Reader
{
public:
    HdfReader();
    ~HdfReader();

public:
    ReaderType getType() { return HDF_READER; }

public:
    int open(const char *filename);
    int close();

public:
    int get_dataset(const char *loc, double **data, int *num, int *dim);
    int get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd);
    int get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs);

public:
    HdfFile h5;
};


#endif
