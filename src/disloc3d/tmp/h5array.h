#ifndef __H5ARRAY_H__
#define __H5ARRAY_H__
#include "hdf5.h"

int h5array_read(hid_t loc_id, const char *name,
                 double **data, int *m, int *n);

int h5array_write(hid_t loc_id, const char *name,
                  double *data, int m, int n);

#endif
