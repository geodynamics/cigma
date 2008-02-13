#ifndef __READER_H__
#define __READER_H__

#include <string>

namespace cigma
{
    class Reader;
}

class cigma::Reader
{
public:
    typedef enum {
        NULL_READER,
        HDF_READER,
        VTK_READER,
        TXT_READER
    } ReaderType;

public:
    Reader();
    ~Reader();

public:
    virtual ReaderType getType() = 0;
    virtual int open(std::string filename) = 0;
    virtual void close() = 0;

public:
    virtual void get_dataset(const char *loc, double **data, int *num, int *dim) = 0;
    virtual void get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd) = 0;
    virtual void get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs) = 0;
};


void new_reader(cigma::Reader **reader, std::string ext);


#endif
