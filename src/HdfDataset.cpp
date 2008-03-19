#include <cassert>
#include <cstdlib>

#include "HdfDataset.h"
#include "HdfAttr.h"


using namespace HdfDataset;



// ---------------------------------------------------------------------------

hid_t HdfDataset::create(hid_t loc_id, const char *name, const char *title,
                         hid_t type_id, int rank, int *shape)
{
    hid_t dataset_id;
    hid_t dataspace_id;
    herr_t status;

    if (rank > 0)
    {
        hsize_t *dims;
        int i;
        dims = (hsize_t *)malloc(rank * sizeof(hsize_t));
        for (i = 0; i < rank; i++)
        {
            dims[i] = shape[i];
        }

        dataspace_id = H5Screate_simple(rank, dims, NULL);
        if (dataspace_id < 0)
        {
            free(dims);
            return -1;
        }
        free(dims);
    }
    else
    {
        return -3;
    }

    dataset_id = H5Dcreate(loc_id, name, type_id, dataspace_id, H5P_DEFAULT);
    if (dataset_id < 0)
    {
        H5Sclose(dataspace_id);
        return -2;
    }

    status = HdfAttr::set_string(dataset_id, "TITLE", title);
    status = HdfAttr::set_string(dataset_id, "CLASS", "ARRAY");
    status = HdfAttr::set_string(dataset_id, "FLAVOR", "numpy");
    status = HdfAttr::set_string(dataset_id, "VERSION", "2.3");

    status = H5Sclose(dataspace_id);

    return dataset_id;

}


hid_t HdfDataset::open(hid_t loc_id, const char *name,
           hid_t *type_id, int *rank, int *shape, int *npoints)
{
    hid_t dataset_id;
    hid_t dataspace_id;
    herr_t status;

    dataset_id = H5Dopen(loc_id, name);
    if (dataset_id < 0)
    {
        return -1;
    }

    if (type_id != NULL)
    {
        *type_id = H5Dget_type(dataset_id);
    }

    if ((rank != NULL) || (shape != NULL) || (npoints != NULL))
    {
        dataspace_id = H5Dget_space(dataset_id);

        if (rank != NULL)
        {
            *rank = H5Sget_simple_extent_ndims(dataspace_id);
        }

        // TODO: think about the shape[] array. should we allocate
        // it here instead of copying it to a fixed array?
        // after all, even the rank is unknown before opening
        // a dataset. note that the type would then have to
        // change to "int **shape"
        if (shape != NULL)
        {
            int i;
            hsize_t *dims;

            dims = (hsize_t *)malloc((*rank) * sizeof(hsize_t));
            status = H5Sget_simple_extent_dims(dataspace_id, dims, NULL);
            for (i = 0; i < *rank; i++)
            {
                shape[i] = (int)dims[i];
            }
            free(dims);
        }

        if (npoints != NULL)
        {
            *npoints = H5Sget_simple_extent_npoints(dataspace_id);
        }

        status = H5Sclose(dataspace_id);
    }

    return dataset_id;
}



// ---------------------------------------------------------------------------

int HdfDataset::read(hid_t loc_id, const char *name,
                     hid_t type_id, int rank, int *shape, void **data)
{
    int dset_rank;
    int dset_npts;
    hid_t dataset_id;
    herr_t status;

    dataset_id = HdfDataset::open(loc_id, name, NULL, &dset_rank, shape, &dset_npts);
    if (dataset_id < 0)
    {
        return -1;
    }
    if (rank != dset_rank)
    {
        H5Dclose(dataset_id);
        return -2;
    }

    *data = malloc(dset_npts * H5Tget_size(type_id));
    if (*data == NULL)
    {
        H5Dclose(dataset_id);
        return -3;
    }

    status = H5Dread(dataset_id, type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, *data);
    if (status < 0)
    {
        H5Dclose(dataset_id);
        return -4;
    }

    status = H5Dclose(dataset_id);

    return 0;
}


int HdfDataset::read1(hid_t loc_id,
                      const char *name,
                      hid_t type_id,
                      void **data, int *n)
{
    return HdfDataset::read(loc_id, name, type_id, 1, n, data);
}


int HdfDataset::read2(hid_t loc_id,
          const char *name,
          hid_t type_id,
          void **data, int *m, int *n)
{
    int shape[2];
    int ret;

    ret = HdfDataset::read(loc_id, name, type_id, 2, shape, data);

    *m = shape[0];
    *n = shape[1];

    return ret;
}

int HdfDataset::read3(hid_t loc_id,
                      const char *name,
                      hid_t type_id,
                      void **data, int *l, int *m, int *n)
{
    int shape[3];
    int ret;

    ret = HdfDataset::read(loc_id, name, type_id, 3, shape, data);

    *l = shape[0];
    *m = shape[1];
    *n = shape[2];

    return ret;
}



// ---------------------------------------------------------------------------

int HdfDataset::write(hid_t loc_id, const char *name, const char *title,
                      hid_t type_id, int rank, int *shape, void *data)
{
    hid_t dataset_id;
    herr_t status;

    dataset_id = HdfDataset::create(loc_id, name, title, type_id, rank, shape);
    if (dataset_id < 0)
    {
        return -1;
    }

    status = H5Dwrite(dataset_id, type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

    if (status < 0)
    {
        H5Dclose(dataset_id);
        return -2;
    }

    H5Dclose(dataset_id);
    return 0;
}


int HdfDataset::write1(hid_t loc_id,
                       const char *name,
                       const char *title,
                       hid_t type_id,
                       void *data, int n)
{
    return HdfDataset::write(loc_id, name, title, type_id, 1, &n, data);
}


int HdfDataset::write2(hid_t loc_id,
                       const char *name,
                       const char *title,
                       hid_t type_id,
                       void *data, int m, int n)
{
    int dims[2] = {m,n};
    return HdfDataset::write(loc_id, name, title, type_id, 2, dims, data);
}


int HdfDataset::write3(hid_t loc_id,
                       const char *name,
                       const char *title,
                       hid_t type_id,
                       void *data, int l, int m, int n)
{
    int dims[3] = {l,m,n};
    return HdfDataset::write(loc_id, name, title, type_id, 3, dims, data);
}


// ---------------------------------------------------------------------------
