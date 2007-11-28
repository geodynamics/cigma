#include <stdlib.h>
#include <hdf5.h>
#include "array.h"
#include "h5_attr.h"



int array_init(array_t *array, hid_t type_id, int rank, int *shape, void *data)
{
    int i;

    // initialize dataspace
    array->rank = rank;
    array->shape = (hsize_t *)malloc(rank * sizeof(hsize_t));
    if (shape != NULL)
    {
        for (i = 0; i < rank; i++)
            array->shape[i] = shape[i];
    }
    else
    {
        for (i = 0; i < rank; i++)
            array->shape[i] = 0;
    }

    // initialize datatype
    array->type_id = type_id;

    // initialize data
    array->data = data;

    return 0;
}

int array_free_dims(array_t *array)
{
    free(array->shape);
    return 0;
}

int array_free_data(array_t *array)
{
    if (array->data != NULL)
    {
        free(array->data);
    }
    return 0;
}

int array_free(array_t *array)
{
    array_free_dims(array);
    array_free_data(array);
    return 0;
}



int array_npoints(array_t *array)
{
    int i;
    int npoints = 1;
    for (i = 0; i < array->rank; i++)
    {
        npoints *= (int) array->shape[i];
    }
    return npoints;
}

int array_dim(array_t *array, int i)
{

    if ((0 <= i) && (i < array->rank))
    {
        return (int)(array->shape[i]);
    }
    return 0;
}

void array_dims(array_t *array, int *rank, int **shape)
{
    int i;

    *rank  = array->rank;
    *shape = (int *)malloc((array->rank) * sizeof(int));
    for (i = 0; i < array->rank; i++)
    {
        (*shape)[i] = (int) array->shape[i];
    }
}

void array_dims1(array_t *array, int *n)
{
    *n = array->shape[0];
}

void array_dims2(array_t *array, int *m, int *n)
{
    *m = array->shape[0];
    *n = array->shape[1];
}

void array_dims3(array_t *array, int *l, int *m, int *n)
{
    *l = array->shape[0];
    *m = array->shape[1];
    *n = array->shape[2];
}



hid_t array_open(array_t *array, hid_t loc_id, const char *name)
{
    hid_t dataset_id;
    hid_t dataspace_id;
    herr_t status;

    dataset_id = H5Dopen(loc_id, name);
    if (dataset_id < 0)
    {
        return -1;
    }

    dataspace_id = H5Dget_space(dataset_id);
    if (dataspace_id < 0)
    {
        H5Dclose(dataset_id);
        return -2;
    }
    
    array->rank = H5Sget_simple_extent_ndims(dataspace_id);

    status = H5Sget_simple_extent_dims(dataspace_id, array->shape, NULL);
    
    status = H5Sclose(dataspace_id);

    return dataset_id;
}

hid_t array_create(array_t *array, hid_t loc_id, const char *name, const char *title)
{
    hid_t dataspace_id;
    hid_t dataset_id;
    herr_t status;

    dataspace_id = H5Screate_simple(array->rank, array->shape, NULL);
    if (dataspace_id < 0)
    {
        return -1;
    }

    dataset_id = H5Dcreate(loc_id, name, array->type_id, dataspace_id,
                           H5P_DEFAULT);
    if (dataset_id < 0)
    {
        H5Sclose(dataspace_id);
        return -2;
    }
    
    status = set_str_attr(dataset_id, "TITLE", title);
    status = set_str_attr(dataset_id, "CLASS", "ARRAY");
    status = set_str_attr(dataset_id, "FLAVOR", "numpy");
    status = set_str_attr(dataset_id, "VERSION", "2.3");

    status = H5Sclose(dataspace_id);

    return dataset_id;
}



int array_read(array_t *array, hid_t loc_id, const char *name)
{
    int npoints;
    hid_t dataset_id;
    herr_t status;

    dataset_id = array_open(array, loc_id, name);
    if (dataset_id < 0)
    {
        return -1;
    }

    npoints = array_npoints(array);

    array->data = malloc(npoints * H5Tget_size(array->type_id));
    if (array->data == NULL)
    {
        H5Dclose(dataset_id);
        return -2;
    }

    status = H5Dread(dataset_id, array->type_id, H5S_ALL, H5S_ALL,
                     H5P_DEFAULT, array->data);
    if (status < 0)
    {
        H5Dclose(dataset_id);
        return -3;
    }

    status = H5Dclose(dataset_id);

    return 0;
}

int array_write(array_t *array, hid_t loc_id, const char *name)
{
    hid_t dataspace_id;
    hid_t dataset_id;
    herr_t status;

    dataspace_id = H5Screate_simple(array->rank, array->shape, NULL);
    if (dataspace_id < 0)
    {
        return -1;
    }

    dataset_id = H5Dcreate(loc_id, name, array->type_id, dataspace_id,
                           H5P_DEFAULT);
    if (dataset_id < 0)
    {
        H5Sclose(dataspace_id);
        return -2;
    }

    status = H5Dwrite(dataset_id, array->type_id, H5S_ALL, H5S_ALL,
                      H5P_DEFAULT, array->data);
    if (status < 0)
    {
        H5Sclose(dataspace_id);
        H5Dclose(dataset_id);
        return -3;
    }

    status = H5Sclose(dataspace_id);
    status = H5Dclose(dataset_id);

    return 0;
}



static herr_t array_hyperslab_init(array_t *array,
                                   hsize_t **offset, hsize_t **stride,
                                   hsize_t **count,  hsize_t **block,
                                   int start, int end)
{
    int i;

    *offset = NULL;
    *stride = NULL;
    *count  = NULL;
    *block  = NULL;

    if ((start < 0) || (start > end))
    {
        return -1;
    }

    if (start >= array->rank)
    {
        return -2;
    }

    if (array->rank == 0)
    {
        return -3;
    }

    *offset = (hsize_t *)malloc((array->rank) * sizeof(hsize_t));
    *stride = (hsize_t *)malloc((array->rank) * sizeof(hsize_t));
    *count  = (hsize_t *)malloc((array->rank) * sizeof(hsize_t));
    *block  = (hsize_t *)malloc((array->rank) * sizeof(hsize_t));

    // first select everything
    for (i = 0; i < array->rank; i++)
    {
        (*offset)[i] = 0;
        (*block )[i] = array->shape[i];
        (*stride)[i] = 1;
        (*count )[i] = 1;
    }

    // now, do the selection on the first dimension only
    (*offset)[0] = start;
    (*block )[0] = end - start;

    return 0;
}

static void array_hyperslab_free(hsize_t *offset, hsize_t *stride,
                                 hsize_t *count,  hsize_t *block)
{
    if (offset != NULL) free(offset);
    if (stride != NULL) free(stride);
    if (count != NULL)  free(count);
    if (block != NULL)  free(block);
}

int array_read_slice(array_t *array, hid_t dataset_id, int start, int end)
{
    hsize_t *offset, *stride, *count, *block;
    hid_t memspace_id;
    hid_t filespace_id;
    herr_t status;

    status = array_hyperslab_init(array,
                                  &offset, &stride,
                                  &count, &block,
                                  start, end);
    if (status < 0)
    {
        return -1;
    }

    memspace_id = H5Screate_simple(array->rank, block, NULL);
    if (memspace_id < 0)
    {
        return -2;
    }

    filespace_id = H5Dget_space(dataset_id);
    if (filespace_id < 0)
    {
        H5Sclose(memspace_id);
        return -3;
    }

    if (array->data == NULL)
    {
        int i;
        int npoints = 1;

        for (i = 0; i < array->rank; i++)
        {
            npoints *= block[i];
        }
        
        array->data = malloc(npoints * H5Tget_size(array->type_id));
        if (array->data == NULL)
        {
            H5Sclose(memspace_id);
            H5Sclose(filespace_id);
            return -4;
        }
    }

    status = H5Sselect_hyperslab(filespace_id, H5S_SELECT_SET,
                                 offset, stride, count, block);
    if (status < 0)
    {
        H5Sclose(memspace_id);
        H5Sclose(filespace_id);
        return -5;
    }

    status = H5Dread(dataset_id, array->type_id, memspace_id, filespace_id,
                     H5P_DEFAULT, array->data);
    if (status < 0)
    {
        H5Sclose(memspace_id);
        H5Sclose(filespace_id);
        return -5;
    }

    status = H5Sclose(memspace_id);
    status = H5Sclose(filespace_id);

    array_hyperslab_free(offset, stride, count, block);

    return 0;
}

int array_write_slice(array_t *array, hid_t dataset_id, int start, int end)
{
    hsize_t *offset, *stride, *count, *block;
    hid_t memspace_id;
    hid_t filespace_id;
    herr_t status;

    status = array_hyperslab_init(array,
                                  &offset, &stride,
                                  &count, &block,
                                  start, end);
    if (status < 0)
    {
        return -1;
    }

    memspace_id = H5Screate_simple(array->rank, block, NULL);
    if (memspace_id < 0)
    {
        return -2;
    }

    filespace_id = H5Dget_space(dataset_id);
    if (filespace_id < 0)
    {
        H5Sclose(memspace_id);
        return -3;
    }

    status = H5Sselect_hyperslab(filespace_id, H5S_SELECT_SET,
                                 offset, stride, count, block);
    if (status < 0)
    {
        H5Sclose(memspace_id);
        H5Sclose(filespace_id);
        return -4;
    }

    status = H5Dwrite(dataset_id, array->type_id, memspace_id, filespace_id,
                      H5P_DEFAULT, array->data);
    if (status < 0)
    {
        H5Sclose(memspace_id);
        H5Sclose(filespace_id);
        return -5;
    }

    status = H5Sclose(memspace_id);
    status = H5Sclose(filespace_id);

    array_hyperslab_free(offset, stride, count, block);

    return 0;
}

