#ifndef __HDF_ATTR_H__
#define __HDF_ATTR_H__

#include "hdf5.h"

namespace HdfAttr
{
    // test whether attribute exists
    herr_t find(hid_t loc_id, const char *attr_name);

    // get dimensions of attribute
    herr_t get_dims(hid_t obj_id, const char *attr_name, int *rank, hsize_t *dims);

    // general get/set methods
    herr_t get(hid_t obj_id, const char *attr_name, hid_t type_id, void *data);
    herr_t set(hid_t obj_id, const char *attr_name, hid_t type_id, const void *data);

    // string get/set methods
    herr_t get_string(hid_t obj_id, const char *attr_name, char **data);
    herr_t set_string(hid_t obj_id, const char *attr_name, const char *attr_data);

    // various other setters
    herr_t set_int(hid_t obj_id, const char *attr_name, int n);
    herr_t set_long(hid_t obj_id, const char *attr_name, long n);
    herr_t set_llong(hid_t obj_id, const char *attr_name, long long n);
    herr_t set_float(hid_t obj_id, const char *attr_name, float n);
    herr_t set_double(hid_t obj_id, const char *attr_name, double n);

    // general array setter
    herr_t set_array(hid_t obj_id, const char *attr_name,
                     size_t rank, hsize_t *dims,
                     hid_t type_id, const void *data);

    // various one-dimensional array setters
    herr_t set_array1(hid_t obj_id, const char *attr_name, hsize_t dim, hid_t type_id, const void *data);
    herr_t set_array1(hid_t obj_id, const char *attr_name, hsize_t dim, const int *data);
    herr_t set_array1(hid_t obj_id, const char *attr_name, hsize_t dim, const float *data);
    herr_t set_array1(hid_t obj_id, const char *attr_name, hsize_t dim, const double *data);
}

#endif
