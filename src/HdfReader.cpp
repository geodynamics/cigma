#include <cstdlib>
#include <cassert>

#include "HdfReader.h"
#include "HdfFile.h"
#include "HdfDataset.h"

using namespace cigma;

// ---------------------------------------------------------------------------

HdfReader::HdfReader()
{
}

HdfReader::~HdfReader()
{
}

// ---------------------------------------------------------------------------


int HdfReader::open(const char *filename)
{
    h5.open(filename, "r");

    if (h5.file_id < 0)
    {
        return -1;
    }

    return 0;
}

int HdfReader::close()
{
    return h5.close();
}

// ---------------------------------------------------------------------------

int HdfReader::get_dataset(const char *loc, double **data, int *num, int *dim)
{
    int rank;
    hid_t type_id;
    H5T_class_t typeclass;
    hid_t dataset_id;

    herr_t status;
    int ierr = -1;

    dataset_id = HdfDataset::open(h5.file_id, loc, &type_id, &rank, NULL, NULL);

    if (dataset_id < 0)
    {
        return -1;
    }

    typeclass = H5Tget_class(type_id);
    if (typeclass != H5T_FLOAT)
    {
        status = H5Dclose(dataset_id);
        return -2;
    }

    if ((rank != 1) && (rank != 2))
    {
        status = H5Dclose(dataset_id);
        return -3;
    }

    status = H5Dclose(dataset_id);

    if (rank == 2)
    {
        ierr = HdfDataset::read2(h5.file_id, loc, H5T_NATIVE_DOUBLE,
                                 (void **)data, num, dim);
        if (ierr < 0)
        {
            status = H5Dclose(dataset_id);
            return -4;
        }
    }
    else if (rank == 1)
    {
        ierr = HdfDataset::read1(h5.file_id, loc, H5T_NATIVE_DOUBLE,
                                 (void **)data, num);
        
        if (ierr < 0)
        {
            status = H5Dclose(dataset_id);
            return -4;
        }

        *dim = 1;
    }
    
    return 0;
}

int HdfReader::get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd)
{
    return this->get_dataset(loc, coordinates, nno, nsd);
}

int HdfReader::get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs)
{
    int rank;
    hid_t type_id;
    H5T_class_t typeclass;
    hid_t connect_id;
    herr_t status;
    int ierr;


    connect_id = HdfDataset::open(h5.file_id, loc, &type_id, &rank, NULL, NULL);
    if (connect_id < 0)
    {
        return -1;
    }

    typeclass = H5Tget_class(type_id);
    if (typeclass != H5T_INTEGER)
    {
        status = H5Dclose(connect_id);
        return -2;
    }

    if (rank != 2)
    {
        status = H5Dclose(connect_id);
        return -3;
    }

    status = H5Dclose(connect_id);

    ierr = HdfDataset::read2(h5.file_id, loc, H5T_NATIVE_INT,
                             (void **)connectivity, nel, ndofs);
}


// ---------------------------------------------------------------------------
