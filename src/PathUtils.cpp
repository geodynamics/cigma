#include <string>
#include "PathUtils.h"
#include "StringUtils.h"

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

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
    return (fileExt == ".vtk")  // legacy format
            || (fileExt == ".vtu")  || (fileExt == ".vts")  || (fileExt == ".vtr")      // XML formats
            || (fileExt == ".pvtu") || (fileExt == ".pvts") || (fileExt == ".pvtr");    // Parallel XML formats
}

bool LibExtension(const char *ext)
{
    string libExt = ext;
    return (libExt == ".so") || (libExt == ".a") || (libExt == ".dll");
}


// ---------------------------------------------------------------------------

//#define S_ISREG(mode)   (mode & _S_IFREG)
//#define S_ISDIR(mode)   (mode & _S_IFDIR)
bool FileExists(const char *filename)
{
    int old_errno = errno;
    bool ret;
    struct stat buf;
    ret = (stat(filename, &buf) == 0) && S_ISREG(buf.st_mode);
    errno = old_errno;
    return ret;
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
