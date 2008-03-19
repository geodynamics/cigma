#ifndef __TEXT_READER_H__
#define __TEXT_READER_H__

#include <cstdio>
#include "Reader.h"

namespace cigma
{
    class TextReader;
}


class cigma::TextReader : public Reader
{
public:
    TextReader();
    ~TextReader();

public:
    ReaderType getType() { return TEXT_READER; }

public:
    int open(const char *filename);
    int close();

public:
    int get_dataset(const char *loc, double **data, int *num, int *dim);
    int get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd);
    int get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs);

public:
    FILE *fp;   // default file pointer
};

// ---------------------------------------------------------------------------

#endif
