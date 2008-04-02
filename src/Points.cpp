#include <limits>
#include <iostream>
#include <cassert>
#include "Points.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

Points::Points()
{
    npts = 0;
    ndim = 0;
    data = 0;
}

Points::~Points()
{
}

// ---------------------------------------------------------------------------


void Points::set_data(double *data, int npts, int ndim)
{
    this->data = data;
    this->npts = npts;
    this->ndim = ndim;
}

void Points::set_locator(Locator *locator)
{
    this->locator = locator;
    locator->initialize(this);
}


// ---------------------------------------------------------------------------

bool Points::find_ann_index(double *point, int *annIndex)
{
    const bool debug = false;

    // assume failure
    *annIndex = -1;

    /* local variables to sort by distance */
    int i,j;
    double r2 = 0;

    int i_min;
    double r2_min;
    double dx[ndim];

    /* first try the locator, if present */
    if (locator != 0)
    {
        int n;
        const double eps = 1e-6;
        const double eps2 = eps*eps;
        double dx[ndim];

        locator->search_point(point);

        i_min = -1;
        r2_min = std::numeric_limits<double>::infinity();
        for (n = 0; n < locator->n_idx(); n++)
        {
            i = locator->idx(n);
            double *pt = &data[ndim*i];

            r2 = 0;
            for (j = 0; j < ndim; j++)
            {
                dx[j] = point[j] - pt[j];
                r2 += dx[j] * dx[j];
            }
            if (debug)
            {
                cerr << "ANN_R2(" << r2 << ")" << endl;
            }
            if (r2 <= eps2)
            {
                //*annIndex = i;
                i_min = i;
                r2_min = r2;
                break;
            }
            if (r2 <= r2_min)
            {
                i_min = i;
                r2_min = r2;
            }
        }
        if (i_min >= 0)
        {
            *annIndex = i_min;
            if (debug)
            {
                cerr << "ANN_R2(" << r2_min << ";" << i_min << ")" << endl;
            }
            return true;
        }

        // XXX: give up here?
        //return false;
    }

    /* Calculate distance to each point one-by-one? */
    i_min = -1;
    r2_min = std::numeric_limits<double>::infinity();
    for (i = 0; i < npts; i++)
    {
        double *pt = &data[ndim*i];

        r2 = 0;
        for (j = 0; j < ndim; j++)
        {
            dx[j] = point[j] - pt[j];
            r2 += dx[j] * dx[j];
        }
        if (r2 <= r2_min)
        {
            i_min = i;
            r2_min = r2;
        }
    }
    if (debug)
    {
        cerr << "R2(" << r2 << ";" << i_min << ")" << endl;
    }
    if (i_min < 0)
    {
        return false;
    }

    /* return best guess */
    *annIndex = i_min;
    return true;
}


// ---------------------------------------------------------------------------
