#include <cassert>
#include <cstdlib>
#include "Reader.h"
#include "HdfReader.h"
#include "TextReader.h"
#include "VtkReader.h"

using namespace cigma;


// ---------------------------------------------------------------------------

void new_reader(cigma::Reader **reader, std::string ext)
{
    if (ext == ".h5")
    {
        *reader = new HdfReader();
        return;
    }

    if (ext == ".txt")
    {
        *reader = new TextReader();
        return;
    }

    if (ext == ".vtk")
    {
        *reader = new VtkReader();
        return;
    }

}

// ---------------------------------------------------------------------------

cigma::Reader::Reader()
{
}

cigma::Reader::~Reader()
{
}

// ---------------------------------------------------------------------------
