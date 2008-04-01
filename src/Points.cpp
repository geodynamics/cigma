#include "Points.h"
#include <limits>

// ---------------------------------------------------------------------------

cigma::Points::Points()
{
    npts = 0;
    ndim = 0;
    data = 0;
}

cigma::Points::~Points()
{
}

// ---------------------------------------------------------------------------


void cigma::Points::set_data(double *data, int npts, int ndim)
{
    this->data = data;
    this->npts = npts;
    this->ndim = ndim;
}

void cigma::Points::set_locator(Locator *locator)
{
    this->locator = locator;
    locator->initialize(this);
}


// ---------------------------------------------------------------------------

bool cigma::Points::find_ann_index(double *point, int *annIndex)
{

    *annIndex = -1;

    if (locator != 0)
    {
        locator->search(point);
        *annIndex = locator->idx(0);
        return true;
    }

    /* Calculate distance to each point one-by-one? */
    int i,j;
    double r2 = 0;

    int i_min;
    double r2_min;

    i_min = -1;
    r2_min = std::numeric_limits<double>::infinity();
    for (i = 0; i < npts; i++)
    {
        double *pt = &data[ndim*i];

        r2 = 0;
        for (j = 0; j < ndim; j++)
        {
            double dx[ndim];
            dx[j] = point[j] - pt[j];
            r2 += dx[j] * dx[j];
        }

        if (r2 <= r2_min)
        {
            i_min = i;
            r2_min = r2;
        }
    }
    if (i_min < 0)
    {
        return false;
    }

    *annIndex = i_min;
    return true;
}


// ---------------------------------------------------------------------------
