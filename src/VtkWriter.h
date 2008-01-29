#ifndef __VTK_WRITER_H__
#define __VTK_WRITER_H__

#include "Writer.h"


namespace cigma
{
    class VtkWriter;
}


class cigma::VtkWriter : public Writer
{
public:
    VtkWriter();
    ~VtkWriter();

public:
    WriterType getType() { return VTK_WRITER; }

};

#endif
