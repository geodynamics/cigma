#include "Points.h"
#include <limits>

// ---------------------------------------------------------------------------

cigma::Points::Points()
{
    num = 0;
    dim = 0;
    data = 0;
}

cigma::Points::~Points()
{
}

// ---------------------------------------------------------------------------


void cigma::Points::set_data(double *data, int num, int dim)
{
    this->data = data;
    this->num = num;
    this->dim = dim;
}

void cigma::Points::set_locator(Locator *locator)
{
    this->locator = locator;
    locator->initialize(this);
}


// ---------------------------------------------------------------------------

bool cigma::Points::find_ann_index(double *globalPoint, int *annIndex)
{

    *annIndex = -1;

    if (locator != 0)
    {
        locator->search(globalPoint);
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
    for (i = 0; i < num; i++)
    {
        double *pt = &data[dim*i];

        r2 = 0;
        for (j = 0; j < dim; j++)
        {
            double dx[dim];
            dx[j] = globalPoint[j] - pt[j];
            r2 += dx[j] * dx[j];
        }

        if (r2 < r2_min)
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
