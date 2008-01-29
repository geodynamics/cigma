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
    virtual void open(std::string filename) = 0;
    virtual void close() = 0;
};

#endif
