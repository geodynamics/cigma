#include <cassert>
#include <cstdlib>
#include "HdfWriter.h"
#include "h5io.h"

using namespace std;


// ---------------------------------------------------------------------------

cigma::HdfWriter::HdfWriter()
{
    file_id = -1;
}

cigma::HdfWriter::~HdfWriter()
{
    close();
}


// ---------------------------------------------------------------------------

void cigma::HdfWriter::
open(std::string filename)
{
    file_id = h5io_file_open(filename.c_str(), "rw");
    if (file_id < 0)
    {
        // XXX: throw exception
    }
}

void cigma::HdfWriter::
close()
{
    if (file_id != -1)
    {
        H5Fclose(file_id);
    }
}


// ---------------------------------------------------------------------------

void cigma::HdfWriter::
write_field(FE_Field *field)
{
    assert(field != 0);
}

// ---------------------------------------------------------------------------
