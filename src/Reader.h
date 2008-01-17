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
        TXT_READER
    } ReaderType;
public:
    Reader();
    ~Reader();
};

#endif
