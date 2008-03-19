#include <cassert>
#include <cstdlib>

#include "Writer.h"
#include "NullWriter.h"
#include "HdfWriter.h"
#include "TextWriter.h"
#include "VtkWriter.h"


using namespace cigma;


// ---------------------------------------------------------------------------

Writer* NewWriter(const char *fileext)
{
    std::string ext = fileext;

    if (ext == ".h5")
    {
        return new HdfWriter();
    }

    if (ext == ".txt")
    {
        return new TextWriter();
    }

    if (ext == ".vtk")
    {
        return new VtkWriter();
    }

    return new NullWriter();
}

// ---------------------------------------------------------------------------

cigma::Writer::Writer()
{
}

cigma::Writer::~Writer()
{
}

// ---------------------------------------------------------------------------
