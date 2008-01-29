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
get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd)
{
    int rank;
    hid_t type_id;
    hid_t coords_id = h5io_dset_open(file_id, loc, &type_id, &rank, NULL, NULL);
    assert(coords_id >= 0);
    assert(rank == 2);
    herr_t status = H5Dclose(coords_id);

    int ierr;
    ierr = h5io_dset_read2(file_id, loc, H5T_NATIVE_DOUBLE,
                           (void **)coordinates, nno, nsd);
    assert(ierr >= 0);
}

void cigma::HdfReader::
get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs)
{
    int rank;
    hid_t type_id;
    hid_t connect_id = h5io_dset_open(file_id, loc, &type_id, &rank, NULL, NULL);
    assert(connect_id >= 0);
    assert(rank == 2);
    herr_t status = H5Dclose(connect_id);

    int ierr;
    ierr = h5io_dset_read2(file_id, loc, H5T_NATIVE_INT,
                           (void **)connectivity, nel, ndofs);
}

void cigma::HdfReader::
get_dataset(const char *loc, double **data, int *num, int *dim)
{
    int rank;
    hid_t type_id;
    hid_t dataset_id = h5io_dset_open(file_id, loc, &type_id, &rank, NULL, NULL);
    assert(dataset_id >= 0);
    assert(rank == 2);
    herr_t status = H5Dclose(dataset_id);

    int ierr;
    ierr = h5io_dset_read2(file_id, loc, H5T_NATIVE_DOUBLE,
                           (void **)data, num, dim);
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
