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
    WriterType getType() { return TXT_WRITER; }
    void open(std::string filename);
    void close();

public:
    void write_field(FE_Field *field);

public:
    void write_connectivity(int *connectivity, int nel, int ndofs);
    void write_coordinates(double *coordinates, int nno, int nsd);
    void write_dofs(double *dofs, int nno, int ndim);

public:
    FILE *fp;
};


// ---------------------------------------------------------------------------

#endif
