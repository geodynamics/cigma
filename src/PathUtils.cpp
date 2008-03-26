#include <string>
#include "PathUtils.h"
#include "StringUtils.h"


using namespace std;


// ---------------------------------------------------------------------------

bool HdfExtension(const char *ext)
{
    string fileExt = ext;
    return (fileExt == ".h5") || (fileExt == ".hdf5");
}


bool TextExtension(const char *ext)
{
    string fileExt = ext;
    return (fileExt == ".txt") || (fileExt == ".dat") || (fileExt == ".out");
}


bool VtkExtension(const char *ext)
{
    string fileExt = ext;
    return (fileExt == ".vtk")
            || (fileExt == ".vtu") || (fileExt == ".vts")
            || (fileExt == ".pvtu") || (fileExt == ".pvts");
}

bool LibExtension(const char *ext)
{
    string libExt = ext;
    return (libExt == ".so") || (libExt == ".a") || (libExt == ".dll");
}


// ---------------------------------------------------------------------------

bool FileExists(const char *filename)
{
    return false;
}

bool IsHdfFile(const char *filename)
{
    return false;
}

bool IsVtkFile(const char *filename)
{
    return false;
}

bool IsLibFile(const char *filename)
{
    return false;
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
