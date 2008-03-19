#include "NullWriter.h"


using namespace cigma;

// ---------------------------------------------------------------------------

NullWriter::NullWriter()
{
}

NullWriter::~NullWriter()
{
}

// ---------------------------------------------------------------------------

int NullWriter::open(const char *filename)
{
    return -1;
}

int NullWriter::close()
{
    return -1;
}

// ---------------------------------------------------------------------------

int NullWriter::write_connectivity(const char *loc, int *connectivity, int nel, int ndofs)
{
    return -1;
}

int NullWriter::write_coordinates(const char *loc, double *coordinates, int nno, int nsd)
{
    return -1;
}

int NullWriter::write_dataset(const char *loc, double *data, int nno, int ndim)
{
    return -1;
}

// ---------------------------------------------------------------------------


