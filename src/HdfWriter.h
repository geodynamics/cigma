#ifndef __HDF_WRITER_H__
#define __HDF_WRITER_H__

#include "Writer.h"


namespace cigma
{
    class HdfWriter;
}


class cigma::HdfWriter : public Writer
{
public:
    HdfWriter();
    ~HdfWriter();

public:
    WriterType getType() { return HDF_WRITER; }

};

#endif
