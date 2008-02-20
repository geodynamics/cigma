#include <cassert>
#include <cstdlib>
#include "Reader.h"
#include "HdfReader.h"
#include "TextReader.h"
#include "VtkReader.h"
#include "VtkXmlReader.h"

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
        // XXX: instantiate legacy vtk reader
        *reader = new VtkReader();
        return;
    }

    if ((ext == ".vts") || (ext == ".vtu"))
    {
        // XXX instantiate xml vtk reader
        *reader = new VtkXmlReader();
        if (ext == ".vtu")
        {
            // XXX: unstructured .vtu file reader still needed...so just fail for now
            assert(false);
        }
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
