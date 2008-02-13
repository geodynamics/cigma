#ifndef __WRITER_H__
#define __WRITER_H__

#include <cstdio>
#include <string>

#include "FE_Field.h"


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
    virtual void open(std::string filename) = 0;
    virtual void close() = 0;

public:
    virtual void write_field(FE_Field *field) = 0;
};


void new_writer(cigma::Writer **writer, std::string ext);


#endif
