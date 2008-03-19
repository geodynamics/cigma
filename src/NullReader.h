#ifndef __NULL_READER_H__
#define __NULL_READER_H__

#include <string>
#include "Reader.h"


namespace cigma
{
    class NullReader;
}


class cigma::NullReader : public Reader
{
public:
    NullReader();
    NullReader(const char *ext);
    ~NullReader();

public:
    ReaderType getType() { return NULL_READER; }

public:
    int open(const char *filename);
    int close();

public:
    int get_dataset(const char *loc, double **data, int *num, int *dim);
    int get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd);
    int get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs);

public:
    std::string ext; // XXX: do we need to keep track of original extension?
};

#endif
