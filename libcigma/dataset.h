#ifndef __CIGMA_DATASET_H__
#define __CIGMA_DATASET_H__

#include <hdf5.h>

#ifdef __cplusplus
extern "C" {
#endif


hid_t dataset_create(hid_t loc_id,
                     const char *name,
                     const char *title,
                     hid_t type_id, int rank, int *shape);

hid_t dataset_open(hid_t loc_id,
                   const char *name,
                   hid_t *type_id, int *rank, int *shape,
                   int *npoints);


int dataset_read(hid_t loc_id,
                 const char *name,
                 hid_t type_id, int rank, int *shape,
                 void **data);


int dataset_write(hid_t loc_id,
                  const char *name,
                  const char *title,
                  hid_t type_id, int rank, int *shape,
                  void *data);


int dataset_read1(hid_t loc_id,
                  const char *name,
                  hid_t type_id,
                  void **data, int *n);

int dataset_read2(hid_t loc_id,
                  const char *name,
                  hid_t type_id,
                  void **data, int *m, int *n);

int dataset_read3(hid_t loc_id,
                  const char *name,
                  hid_t type_id,
                  void **data, int *l, int *m, int *n);


int dataset_write1(hid_t loc_id,
                   const char *name,
                   const char *title,
                   hid_t type_id,
                   void *data, int n);

int dataset_write2(hid_t loc_id,
                   const char *name,
                   const char *title,
                   hid_t type_id,
                   void *data, int m, int n);

int dataset_write3(hid_t loc_id,
                   const char *name,
                   const char *title,
                   hid_t type_id,
                   void *data, int l, int m, int n);


int dataset_read3_slice(hid_t loc_id,
                        const char *name,
                        hid_t type_id,
                        int rank, hsize_t *dims, void *data,
                        int start, int end);

int dataset_write3_slice(hid_t loc_id,
                         const char *name,
                         const char *title,
                         hid_t type_id,
                         int start, int end);

#ifdef __cplusplus
}
#endif

#endif /* __CIGMA_DATASET_H__ */
