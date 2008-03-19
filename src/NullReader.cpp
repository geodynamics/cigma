#include "NullReader.h"

using namespace cigma;


// ---------------------------------------------------------------------------

NullReader::NullReader()
{
    this->ext = "";
}

NullReader::NullReader(const char *ext)
{
    this->ext = ext;
}

NullReader::~NullReader()
{
}

// ---------------------------------------------------------------------------

int NullReader::open(const char *filename)
{
    return -1;
}

int NullReader::close()
{
    return -1;
}

// ---------------------------------------------------------------------------

int NullReader::get_dataset(const char *loc, double **data, int *num, int *dim)
{
    return -1;
}

int NullReader::get_coordinates(const char *loc, double **coordinates, int *nno, int *nsd)
{
    return -1;
}

int NullReader::get_connectivity(const char *loc, int **connectivity, int *nel, int *ndofs)
{
    return -1;
}

// ---------------------------------------------------------------------------
