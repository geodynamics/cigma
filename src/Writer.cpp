#include <cassert>
#include <cstdlib>

#include "Writer.h"
#include "NullWriter.h"
#include "HdfWriter.h"
#include "TextWriter.h"
#include "VtkWriter.h"
#include "PathUtils.h"

using namespace cigma;

// ---------------------------------------------------------------------------

Writer* NewWriter(const char *fileext)
{
    // 
    // XXX: This factory function also needs to accept a flag indicating
    // whether we wish to overwrite the file if it exists. Thus, we need
    // the full filename, not just the extension part.
    //

    if (HdfExtension(fileext))
    {
        return new HdfWriter();
    }

    if (TextExtension(fileext))
    {
        return new TextWriter();
    }

    if (VtkExtension(fileext))
    {
        return new VtkWriter();
    }

    return new NullWriter();
}

// ---------------------------------------------------------------------------

Writer::Writer()
{
}

Writer::~Writer()
{
}

// ---------------------------------------------------------------------------
