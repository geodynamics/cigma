#ifndef __WRITER_H__
#define __WRITER_H__

#include <cstdio>
#include <string>


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
        TXT_WRITER
    } WriterType;

public:
    Writer();
    ~Writer();

public:
    virtual WriterType getType() = 0;
};

// ---------------------------------------------------------------------------

#endif
