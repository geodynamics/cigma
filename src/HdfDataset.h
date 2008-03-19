#ifndef __HDF_DATASET_H__
#define __HDF_DATASET_H__

#include "hdf5.h"

namespace HdfDataset
{

    hid_t create(hid_t loc_id, const char *name, const char *title,
                 hid_t type_id, int rank, int *shape);

    hid_t open(hid_t loc_id, const char *name,
               hid_t *type_id, int *rank, int *shape, int *npoints);

    // -----------------------------------------------------------------------

    int read(hid_t loc_id, const char *name,
             hid_t type_id, int rank, int *shape, void **data);

    int read1(hid_t loc_id,
              const char *name,
              hid_t type_id,
              void **data, int *n);

    int read2(hid_t loc_id,
              const char *name,
              hid_t type_id,
              void **data, int *m, int *n);

    int read3(hid_t loc_id,
              const char *name,
              hid_t type_id,
              void **data, int *l, int *m, int *n);

    // -----------------------------------------------------------------------

    int write(hid_t loc_id, const char *name, const char *title,
              hid_t type_id, int rank, int *shape, void *data);

    int write1(hid_t loc_id,
               const char *name,
               const char *title,
               hid_t type_id,
               void *data, int n);

    int write2(hid_t loc_id,
               const char *name,
               const char *title,
               hid_t type_id,
               void *data, int m, int n);

    int write3(hid_t loc_id,
               const char *name,
               const char *title,
               hid_t type_id,
               void *data, int l, int m, int n);

}

#endif
