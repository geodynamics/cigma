#include <string>
#include <cstdlib>
#include <cassert>

#include "Reader.h"
#include "NullReader.h"
#include "HdfReader.h"
#include "TextReader.h"
#include "VtkReader.h"

using namespace cigma;


// ---------------------------------------------------------------------------

cigma::Reader* NewReader(const char *fileext)
{
    std::string ext = fileext;

    if (ext == ".h5")
    {
        return new HdfReader();
    }

    if (ext == ".txt")
    {
        return new TextReader();
    }

    if ((ext == ".vtk") || (ext == ".vts") || (ext == ".vtu"))
    {
        return new VtkReader();
    }

    return new NullReader(fileext);
}

// ---------------------------------------------------------------------------

cigma::Reader::Reader()
{
}

cigma::Reader::~Reader()
{
}

// ---------------------------------------------------------------------------
