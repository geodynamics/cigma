#include <cassert>
#include <cstdlib>
#include "Writer.h"
#include "HdfWriter.h"
#include "TextWriter.h"
#include "VtkWriter.h"


using namespace cigma;


// ---------------------------------------------------------------------------

void new_writer(cigma::Writer **writer, std::string ext)
{
    if (ext == ".h5")
    {
        *writer = new HdfWriter();
        return;
    }

    if (ext == ".txt")
    {
        *writer = new TextWriter();
        return;
    }

    if (ext == ".vtk")
    {
        *writer = new VtkWriter();
        return;
    }

}

// ---------------------------------------------------------------------------

cigma::Writer::Writer()
{
}

cigma::Writer::~Writer()
{
}

// ---------------------------------------------------------------------------
