#ifndef __TEXTWRITER_H__
#define __TEXTWRITER_H__

#include <cstdio>
#include <string>

namespace cigma
{
    class TextWriter;
};

class cigma::TextWriter
{
public:
    TextWriter();
    ~TextWriter();

public:
    void open(std::string filename);
    void close();

public:
    void write_connectivity(int *connectivity, int nel, int ndofs);
    void write_coordinates(double *coordinates, int nno, int nsd);
    void write_dofs(double *dofs, int nno, int ndim);

public:
    FILE *fp;
};

// ---------------------------------------------------------------------------

#endif
