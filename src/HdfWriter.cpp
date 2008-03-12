#include <cassert>
#include <cstdlib>
#include "HdfWriter.h"
#include "h5io.h"

using namespace std;
using namespace cigma;


// ---------------------------------------------------------------------------

HdfWriter::HdfWriter()
{
    file_id = -1;
}

HdfWriter::~HdfWriter()
{
    close();
}


// ---------------------------------------------------------------------------

int HdfWriter::open(string filename)
{
    file_id = h5io_file_open(filename.c_str(), "rw+");

    if (file_id < 0)
    {
        return -1;
    }
    return 0;
}

void HdfWriter::close()
{
    herr_t status;
    if (file_id != -1)
    {
        status = H5Fclose(file_id);
    }
    file_id = -1;
}


// ---------------------------------------------------------------------------

int HdfWriter::write_coordinates(const char *loc, double *coordinates, int nno, int nsd)
{
    int ierr;
    ierr = h5io_dset_write2(file_id, loc, "coordinates array", H5T_NATIVE_DOUBLE, coordinates, nno, nsd);
    return ierr;
}

int HdfWriter::write_connectivity(const char *loc, int *connectivity, int nel, int ndofs)
{
    int ierr;
    ierr = h5io_dset_write2(file_id, loc, "connectivity array", H5T_NATIVE_INT, connectivity, nel, ndofs);
    return ierr;
}

// ---------------------------------------------------------------------------

void cigma::HdfWriter::
write_field(FE_Field *field)
{
    assert(field != 0);
}

// ---------------------------------------------------------------------------
