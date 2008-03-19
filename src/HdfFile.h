#ifndef __HDF_FILE_H__
#define __HDF_FILE_H__

#include "hdf5.h"

class HdfFile
{
public:
    HdfFile();
    ~HdfFile();

public:
    int create(const char *filename, const char *mode);
    int open(const char *filename, const char *mode);
    int close();

public:
    hid_t file_id;
};

#endif
