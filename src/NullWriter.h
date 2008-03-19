#ifndef __NULL_WRITER_H__
#define __NULL_WRITER_H__

#include "Writer.h"

namespace cigma
{
    class NullWriter;
}

class cigma::NullWriter : public Writer
{
public:
    NullWriter();
    ~NullWriter();

public:
    WriterType getType() { return TEXT_WRITER; }

public:
    int open(const char *filename);
    int close();

public:
    int write_dataset(const char *loc, double *data, int nno, int ndim);
    int write_connectivity(const char *loc, int *connectivity, int nel, int ndofs);
    int write_coordinates(const char *loc, double *coordinates, int nno, int nsd);
};

#endif
