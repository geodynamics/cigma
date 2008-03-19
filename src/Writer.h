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
};


cigma::Writer* NewWriter(const char *ext);


#endif
