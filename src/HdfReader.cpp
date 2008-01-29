#include "HdfReader.h"
#include "h5io.h"
#include <cstdlib>
#include <cassert>

// ---------------------------------------------------------------------------

cigma::HdfReader::HdfReader()
{
}

cigma::HdfReader::~HdfReader()
{
}

// ---------------------------------------------------------------------------


void cigma::HdfReader::
open(std::string filename)
{
    file_id = h5io_file_open(filename.c_str(), "r");
    assert(file_id >= 0);
}


void cigma::HdfReader::
close()
{
    herr_t status = H5Fclose(file_id);
}

// ---------------------------------------------------------------------------


void cigma::HdfReader::
set_coordinates_path(const char *loc)
{
    coords_path = loc;
}

void cigma::HdfReader::
set_connectivity_path(const char *loc)
{
    connect_path = loc;
}

void cigma::HdfReader::
set_dataset_path(const char *loc)
{
    dataset_path = loc;
}

// ---------------------------------------------------------------------------

void cigma::HdfReader::
get_coordinates(double **coordinates, int *nno, int *nsd)
{
    int rank;
    const char *path = coords_path.c_str();
    coords_id = h5io_dset_open(file_id, path, &coords_type, &rank, NULL, NULL);
    assert(coords_id >= 0);
    assert(rank == 2);

    int ierr;
    ierr = h5io_dset_read2(file_id, path, H5T_NATIVE_DOUBLE,
                           (void **)coordinates, nno, nsd);
    assert(ierr >= 0);
}

void cigma::HdfReader::
get_connectivity(int **connectivity, int *nel, int *ndofs)
{
    int rank;
    const char *path = connect_path.c_str();
    connect_id = h5io_dset_open(file_id, path, &connect_type, &rank, NULL, NULL);
    assert(connect_id >= 0);
    assert(rank == 2);

    int ierr;
    ierr = h5io_dset_read2(file_id, path, H5T_NATIVE_INT,
                           (void **)connectivity, nel, ndofs);
}

void cigma::HdfReader::
get_dataset(double **data, int *num, int *dim)
{
    int rank;
    const char *path = dataset_path.c_str();
    dataset_id = h5io_dset_open(file_id, path, &dataset_type, &rank, NULL, NULL);
    assert(dataset_id >= 0);
    assert(rank == 2);

    int ierr;
    ierr = h5io_dset_read2(file_id, path, H5T_NATIVE_DOUBLE,
                           (void **)data, num, dim);
}


// ---------------------------------------------------------------------------
