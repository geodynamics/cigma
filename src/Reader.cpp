#include <string>
#include <cstdlib>
#include <cassert>

#include "Reader.h"
#include "NullReader.h"
#include "HdfReader.h"
#include "TextReader.h"
#include "VtkReader2.h"

#include "PathUtils.h"


using namespace cigma;


// ---------------------------------------------------------------------------

Reader* NewReader(const char *fileext)
{
    // 
    // XXX: Need to pass filename to this function, not just the extension.
    // This way, we could use the IsHdfFile() and IsVtkFile() functions
    // which can read the corresponding file headers to verify.
    // Another thing to verify here is whether the file exists or not.
    //

    if (HdfExtension(fileext))
    {
        return new HdfReader();
    }

    if (TextExtension(fileext))
    {
        return new TextReader();
    }

    if (VtkExtension(fileext))
    {
        return new VtkReader();
    }

    return new NullReader(fileext);
}

// ---------------------------------------------------------------------------

Reader::Reader()
{
}

Reader::~Reader()
{
}

// ---------------------------------------------------------------------------
