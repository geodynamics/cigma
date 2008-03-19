#ifndef __TEXTWRITER_H__
#define __TEXTWRITER_H__

#include <cstdio>
#include <string>

#include "Writer.h"


namespace cigma
{
    class TextWriter;
};


class cigma::TextWriter : public Writer
{
public:
    TextWriter();
    ~TextWriter();

public:
    WriterType getType() { return TEXT_WRITER; }

public:
    int open(const char *filename);
    int close();

public:
    int write_connectivity(int *connectivity, int nel, int ndofs);
    int write_coordinates(double *coordinates, int nno, int nsd);
    int write_dataset(double *data, int nno, int ndim);

public:
    FILE *fp;
};


// ---------------------------------------------------------------------------

#endif
