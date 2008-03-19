#ifndef __READER_H__
#define __READER_H__

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
        TEXT_READER
    } ReaderType;


public:
    Reader();
    ~Reader();

public:
    virtual ReaderType getType() = 0;

public:
    virtual int open(const char *filename) = 0;
    virtual int close() = 0;

public:
    virtual int get_dataset(const char *loc, double **data, int *num, int *dim) = 0;
    virtual int get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd) = 0;
    virtual int get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs) = 0;
};

cigma::Reader* NewReader(const char *ext);

#endif
