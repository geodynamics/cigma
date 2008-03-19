#include "HdfArray.h"
#include <cassert>
#include <cstdlib>


// ---------------------------------------------------------------------------

HdfArray::HdfArray()
{
    rank = 0;
    shape = 0;
    data = 0;
}

HdfArray::~HdfArray()
{
    if (shape != 0) free(shape);
    //if (data != 0) free(data);
}

// ---------------------------------------------------------------------------

void HdfArray::initialize(hid_t type_id, int rank, int *shape)
{
    int i;

    /* initialize dataspace */
    this->rank = rank;
    this->shape = (hsize_t *)malloc(rank * sizeof(hsize_t));
    if (shape != NULL)
    {
        for (i = 0; i < rank; i++)
        {
            this->shape[i] = shape[i];
        }
    }
    else
    {
        for (i = 0; i < rank; i++)
        {
            this->shape[i] = 0;
        }
    }

    /* initialize data type */
    this->type_id = type_id;

}

