#ifndef __HDF_GROUP_H__
#define __HDF_GROUP_H__

#include "hdf5.h"

class HdfGroup
{
public:
    HdfGroup(hid_t loc_id, const char *name);
    ~HdfGroup();

private:
    HdfGroup(const HdfGroup &);

public:
    bool exists();


public:
    hid_t group_id;

};

// ---------------------------------------------------------------------------

inline bool HdfGroup::exists()
{
    return (group_id >= 0);
}

// ---------------------------------------------------------------------------

#endif
