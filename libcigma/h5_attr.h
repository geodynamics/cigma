#ifndef __CIGMA_ATTR_H__
#define __CIGMA_ATTR_H__
#include <hdf5.h>

#ifdef __cplusplus
extern "C" {
#endif


herr_t find_attr(hid_t loc_id, const char *attr_name);
herr_t set_attr(hid_t obj_id, const char *attr_name, hid_t type_id, const void *data);
herr_t get_attr(hid_t obj_id, const char *attr_name, hid_t type_id, void *data);
herr_t get_attr_dims(hid_t obj_id, const char *attr_name, int *rank, hsize_t *dims);


herr_t set_str_attr(hid_t obj_id, const char *attr_name, const char *attr_data);
herr_t get_str_attr(hid_t obj_id, const char *attr_name, char **data);


herr_t set_float_attr(hid_t obj_id, const char *attr_name, float x);
herr_t set_double_attr(hid_t obj_id, const char *attr_name, double x);
herr_t set_int_attr(hid_t obj_id, const char *attr_name, int n);
herr_t set_long_attr(hid_t obj_id, const char *attr_name, long n);
herr_t set_llong_attr(hid_t obj_id, const char *attr_name, long long n);


herr_t set_array_attr(hid_t obj_id, const char *attr_name,
                      size_t rank, hsize_t *dims, hid_t type_id, const void *data);

herr_t set_array1_attr(hid_t obj_id, const char *attr_name,
                       hsize_t dim, hid_t type_id, const void *data);

herr_t set_int_array_attr(hid_t obj_id, const char *attr_name,
                          hsize_t dim, const int *data);

herr_t set_float_array_attr(hid_t obj_id, const char *attr,
                            hsize_t dim, const float *data);

herr_t set_double_array_attr(hid_t obj_id, const char *attr,
                             hsize_t dim, const double *data);

#ifdef __cplusplus
}
#endif
#endif
