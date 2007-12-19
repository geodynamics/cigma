#ifndef __TEXTREADER_H__
#define __TEXTREADER_H__

#include <cstdio>
#include <string>

namespace cigma
{
    class TextReader;
};

class cigma::TextReader
{
public:
    TextReader();
    ~TextReader();

public:
    void open(std::string filename);
    void close();

public:
    void get_connectivity(int **connectivity, int *nel, int *ndofs);
    void get_coordinates(double **coordinates, int *nno, int *nsd);
    void get_dofs(double **dofs, int *nno, int *ndim);

public:
    FILE *fp;
};

// ---------------------------------------------------------------------------

#endif
