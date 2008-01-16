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
    ierr = h5io_dset_read2(file_id, path, coords_type,
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
    ierr = h5io_dset_read2(file_id, path, connect_type,
                           (void **)connectivity, nel, ndofs);
}

void cigma::HdfReader::
get_point_data(const char *name, double **data, int *num, int *dim)
{
    int rank;
    const char *path = dataset_path.c_str();
    dataset_id = h5io_dset_open(file_id, path, &dataset_type, &rank, NULL, NULL);
    assert(dataset_id >= 0);
    assert(rank == 2);

    int ierr;
    ierr = h5io_dset_read2(file_id, path, dataset_type,
                           (void **)data, num, dim);
}


// ---------------------------------------------------------------------------
