#ifndef __CIGMA_H5IO_H__
#define __CIGMA_H5IO_H__

#include <hdf5.h>

#ifdef __cplusplus
extern "C" {
#endif

hid_t h5_file_create(const char *filename, const char *mode);
hid_t h5_file_open(const char *filename, const char *mode);

hid_t h5_group_create(hid_t loc_id, const char *name);
hid_t h5_group_open(hid_t loc_id, const char *name);

hid_t h5_touch_group(hid_t loc_id, const char *name);
hid_t h5_touch_path(hid_t loc_id, const char *path);

#ifdef __cplusplus
}
#endif

#endif /* __CIGMA_H5IO_H__ */
