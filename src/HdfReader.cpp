#include "HdfReader.h"
#include "h5io.h"
#include <cstdlib>
#include <cassert>

// ---------------------------------------------------------------------------

cigma::HdfReader::HdfReader()
{
    file_id = -1;
}

cigma::HdfReader::~HdfReader()
{
}

// ---------------------------------------------------------------------------


int cigma::HdfReader::
open(std::string filename)
{
    file_id = h5io_file_open(filename.c_str(), "r");
    assert(file_id >= 0);
    return 0; // XXX: change return value instead of using assert
}


void cigma::HdfReader::
close()
{
    herr_t status = H5Fclose(file_id);
    if (status < 0)
    {
        // XXX: emit warning?
    }
}

// ---------------------------------------------------------------------------

void cigma::HdfReader::
get_dataset(const char *loc, double **data, int *num, int *dim)
{
    int rank;
    hid_t type_id;
    hid_t dataset_id;
    herr_t status;
    int ierr;

    dataset_id = h5io_dset_open(file_id, loc, &type_id, &rank, NULL, NULL);
    assert(H5Tget_class(type_id) == H5T_FLOAT);
    assert(dataset_id >= 0);
    assert((rank == 1) || (rank == 2));
    status = H5Dclose(dataset_id);
    assert(status >= 0); // XXX: emit warning?

    if (rank == 2)
    {
        ierr = h5io_dset_read2(file_id, loc, H5T_NATIVE_DOUBLE,
                               (void **)data, num, dim);
    }
    else if (rank == 1)
    {
        ierr = h5io_dset_read1(file_id, loc, H5T_NATIVE_DOUBLE,
                               (void **)data, num);
        *dim = 1;
    }
    assert(ierr >= 0);
}

void cigma::HdfReader::
get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd)
{
    get_dataset(loc, coordinates, nno, nsd);
}

void cigma::HdfReader::
get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs)
{
    int rank;
    hid_t type_id;
    hid_t connect_id;
    herr_t status;

    connect_id = h5io_dset_open(file_id, loc, &type_id, &rank, NULL, NULL);
    assert(H5Tget_class(type_id) == H5T_INTEGER);
    assert(connect_id >= 0);
    assert(rank == 2);
    status = H5Dclose(connect_id);
    if (status < 0)
    {
        // XXX: emit warning
    }

    int ierr;
    ierr = h5io_dset_read2(file_id, loc, H5T_NATIVE_INT,
                           (void **)connectivity, nel, ndofs);
}


// ---------------------------------------------------------------------------

void cigma::HdfReader::
get_mesh(MeshPart *meshPart, const char *loc)
{
    // XXX: read coords_loc & connect_loc from metadata
}

void cigma::HdfReader::
get_mesh(MeshPart *meshPart, const char *coords_loc, const char *connect_loc)
{
    assert(meshPart != 0);
    get_coordinates(coords_loc, &(meshPart->coords), &(meshPart->nno), &(meshPart->nsd));
    get_connectivity(connect_loc, &(meshPart->connect), &(meshPart->nel), &(meshPart->nel));
}

void cigma::HdfReader::
get_dofs(DofHandler *dofHandler, const char *loc)
{
    assert(dofHandler != 0);
    assert(dofHandler->meshPart != 0);
    get_dataset(loc, &(dofHandler->dofs), &(dofHandler->nno), &(dofHandler->ndim));
}



// ---------------------------------------------------------------------------
