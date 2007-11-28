#ifndef __CIGMA_ARRAY_H__
#define __CIGMA_ARRAY_H__
#include <hdf5.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    int rank;
    hsize_t *shape;
    hid_t type_id;
    void *data;
} array_t;



int array_init(array_t *array, hid_t type_id, int rank, int *shape, void *data);
int array_free_dims(array_t *array);
int array_free_data(array_t *array);
int array_free(array_t *array);


int array_npoints(array_t *array);
int array_dim(array_t *array, int i);
void array_dims(array_t *array, int *rank, int **shape);
void array_dims1(array_t *array, int *n);
void array_dims2(array_t *array, int *m, int *n);
void array_dims3(array_t *array, int *l, int *m, int *n);


hid_t array_open(array_t *array, hid_t loc_id, const char *name);
hid_t array_create(array_t *array, hid_t loc_id, const char *name, const char *title);


int array_read(array_t *array, hid_t loc_id, const char *name);
int array_write(array_t *array, hid_t loc_id, const char *name);


int array_read_slice(array_t *array, hid_t dataset_id, int start, int end);
int array_write_slice(array_t *array, hid_t dataset_id, int start, int end);


#ifdef __cplusplus
}
#endif
#endif /* __CIGMA_H5_ARRAY_H__ */
