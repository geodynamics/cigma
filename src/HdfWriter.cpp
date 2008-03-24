#include <cassert>
#include <cstdlib>

#include "HdfWriter.h"
#include "HdfDataset.h"

using namespace std;
using namespace cigma;


// ---------------------------------------------------------------------------

HdfWriter::HdfWriter()
{
}

HdfWriter::~HdfWriter()
{
}


// ---------------------------------------------------------------------------

int HdfWriter::open(const char *filename)
{
    h5.open(filename, "rw+");

    if (h5.file_id < 0)
    {
        return -1;
    }

    return 0;
}

int HdfWriter::close()
{
    return h5.close();
}


// ---------------------------------------------------------------------------

int HdfWriter::write_dataset(const char *loc, double *data, int nno, int ndim)
{
    int ierr;
    ierr = HdfDataset::write2(h5.file_id, loc, "dataset array", H5T_NATIVE_DOUBLE, (void **)data, nno, ndim);
    return ierr;
}

int HdfWriter::write_coordinates(const char *loc, double *coordinates, int nno, int nsd)
{
    int ierr;
    ierr = HdfDataset::write2(h5.file_id, loc, "coordinates array", H5T_NATIVE_DOUBLE, coordinates, nno, nsd);
    return ierr;
}

int HdfWriter::write_connectivity(const char *loc, int *connectivity, int nel, int ndofs)
{
    int ierr;
    ierr = HdfDataset::write2(h5.file_id, loc, "connectivity array", H5T_NATIVE_INT, connectivity, nel, ndofs);
    return ierr;
}


// ---------------------------------------------------------------------------

int HdfWriter::write_int_dataset(const char *loc, int *data, int rows, int cols)
{
    int ierr;
    ierr = HdfDataset::write2(h5.file_id, loc, "", H5T_NATIVE_INT, data, rows, cols);
    return ierr;
}


// ---------------------------------------------------------------------------
