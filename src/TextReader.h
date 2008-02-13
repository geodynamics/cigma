#ifndef __TEXTREADER_H__
#define __TEXTREADER_H__

#include <cstdio>
#include <string>

#include "Reader.h"

namespace cigma
{
    class TextReader;
};

class cigma::TextReader : public Reader
{
public:
    TextReader();
    ~TextReader();

public:
    ReaderType getType() { return TXT_READER; }

public:
    int open(std::string filename);
    void close();

public:
    void get_dataset(const char *loc, double **data, int *num, int *dim);
    void get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd);
    void get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs);

public:
    FILE *fp;
};

// ---------------------------------------------------------------------------

#endif
