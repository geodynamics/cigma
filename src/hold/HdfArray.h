#ifndef __HDF_ARRAY_H__
#define __HDF_ARRAY_H__

#include "hdf5.h"


class HdfArray
{
public:
    HdfArray();
    ~HdfArray();


public:
    void set_shape(hid_t type_id, int rank, int *shape);

public:
    int create(hid_t loc_id, const char *name, const char *title);
    int open(hid_t loc_id, const char *name);

public:
    int read(hid_t loc_id, const char *name);
    int write(hid_t loc_id, const char *name);

public:
    int slice_read(int start, int end);
    int slice_write(int start, int end);

public:
    int rank;
    hsize_t *shape;
    hid_t type_id;
    void *data;

public:
    hid_t dataset_id;
    hsize_t *offset;
    hsize_t *stride;
    hsize_t *count;
    hsize_t *block;

private:
    HdfArray(const HdfArray &);

};

#endif
