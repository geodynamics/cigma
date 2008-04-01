#include <string>
#include <cassert>

#include "Field.h"

#include "NullField.h"
#include "FE_Field.h"
#include "PointField.h"

#include "StringUtils.h"
#include "PathUtils.h"
#include "HdfAttr.h"


using namespace std;
using namespace cigma;


// ---------------------------------------------------------------------------

Field *NewField(const char *src)
{
    string fieldPath = src;
    string fieldLoc, fieldFile, fieldExt;
    parse_dataset_path(fieldPath, fieldLoc, fieldFile, fieldExt);

    //
    // At this point, we need to decide what kind of field object we'll be
    // creating. For the most part, we will use a FE_Field, but we'll use
    // metadata attributes in HDF5 files to instantiate other types of fields.
    //
    if (HdfExtension(fieldExt.c_str()))
    {
        // 
        // Make sure that fieldFile is an HDF5 file (use IsHdfFile())
        // read attribute from fieldLoc to distinguish between FE_Field and PointField
        // (use HdfAttr::get_string())
        //
        if (fieldExt == ".hdf5") // XXX: temporary hack for choosing PointField
        {
            return new PointField();
        }
        return new FE_Field();
    }

    if (VtkExtension(fieldExt.c_str()))
    {
        // make sure that fieldFile is a VTK file (use IsVtkFile())
        return new FE_Field();
    }

    if (TextExtension(fieldExt.c_str()))
    {
        // assume text file contains dofs for a FE_Field
        return new FE_Field();
    }

    return new NullField();
}


// ---------------------------------------------------------------------------

Field::~Field() {};


// ---------------------------------------------------------------------------

bool Field::eval(Points &domain, Points &range)
{
    assert(domain.n_points() == range.n_points());
    for (int i = 0; i < domain.n_points(); i++)
    {
        bool ret;
        double *x = domain[i];
        double *y = range[i];
        ret = eval(x,y);
        if (ret)
            return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
