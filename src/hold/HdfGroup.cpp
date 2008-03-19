#include "HdfGroup.h"
#include <cassert>

// ---------------------------------------------------------------------------

HdfGroup::HdfGroup(hid_t loc_id, const char *name)
{
    herr_t status;

    group_id = H5Gcreate(loc_id, name, 0);
    if (group_id < 0)
    {

    }
}

HdfGroup::~HdfGroup()
{
    herr_t status;

    if (group_id < 0)
    {
        status = H5Gclose(group_id);
    }

    group_id = -1;
}

// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------

