#ifndef __WRITER_H__
#define __WRITER_H__


namespace cigma
{
    class Writer;
};


class cigma::Writer
{
public:
    typedef enum {
        NULL_WRITER,
        HDF_WRITER,
        VTK_WRITER,
        TEXT_WRITER
    } WriterType;

public:
    Writer();
    ~Writer();

public:
    virtual WriterType getType() = 0;
    virtual int open(const char *filename) = 0;
    virtual int close() = 0;

public:
    virtual int write_dataset(const char *loc, double *data, int nno, int ndim) = 0;
    virtual int write_coordinates(const char *loc, double *coordinates, int nno, int nsd) = 0;
};


cigma::Writer* NewWriter(const char *ext);


#endif
