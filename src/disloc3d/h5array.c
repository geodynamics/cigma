#include <stdlib.h>
#include "hdf5.h"
#include "h5array.h"

int h5array_read(hid_t loc_id, const char *name,
                 double **data, int *m, int *n)
{
    hid_t dataset_id;
    hid_t dataspace_id;
    hsize_t dims[2];
    herr_t status;

    int rank;


    dataset_id = H5Dopen(loc_id, name);
    if (dataset_id < 0)
    {
        return -1;
    }

    dataspace_id = H5Dget_space(dataset_id);
    rank = H5Sget_simple_extent_ndims(dataspace_id);
    if (rank != 2)
    {
        H5Dclose(dataset_id);
        H5Sclose(dataspace_id);
        return -2;
    }

    status = H5Sget_simple_extent_dims(dataspace_id, dims, NULL);
    status = H5Sclose(dataspace_id);

    *m = dims[0];
    *n = dims[1];

    *data = (double *)malloc((dims[0] * dims[1]) * sizeof(double));
    if (*data == NULL)
    {
        H5Dclose(dataset_id);
        return -3;
    }

    status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL,
                     H5P_DEFAULT, *data);
    if (status < 0)
    {
        H5Dclose(dataset_id);
        return -4;
    }

    status = H5Dclose(dataset_id);

    return 0;
}

int h5array_write(hid_t loc_id, const char *name,
                  double *data, int m, int n)
{
    hsize_t dims[2];
    hid_t dataspace_id;
    hid_t dataset_id;
    herr_t status;

    dims[0] = m;
    dims[1] = n;
    dataspace_id = H5Screate_simple(2, dims, NULL);
    if (dataspace_id < 0)
    {
        return -1;
    }

    dataset_id = H5Dcreate(loc_id, name, H5T_NATIVE_DOUBLE, dataspace_id,
                           H5P_DEFAULT);
    if (dataset_id < 0)
    {
        H5Sclose(dataspace_id);
        return -2;
    }

    status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL,
                      H5P_DEFAULT, data);
    if (status < 0)
    {
        H5Sclose(dataspace_id);
        H5Dclose(dataset_id);
        return -3;
    }

    H5Sclose(dataspace_id);
    H5Dclose(dataset_id);

    return 0;
}

