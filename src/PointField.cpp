#include <cassert>
#include "PointField.h"

using namespace cigma;

// ---------------------------------------------------------------------------

PointField::PointField()
{
    points = new Points();
    values = new Points();
}

PointField::~PointField()
{
    delete points;
    delete values;
}

// ---------------------------------------------------------------------------

void PointField::set_points(double *pts, int npts, int nsd)
{
    points->set_data(pts, npts, nsd);
}

void PointField::set_values(double *vals, int nvals, int rank)
{
    values->set_data(vals, nvals, rank);
}


// ---------------------------------------------------------------------------

bool PointField::eval(double *point, double *value)
{
    /* XXX: quick sanity check
    static int checked = 0;
    if (!checked)
    {
        assert(points->n_points() != 0);
        assert(values->n_points() != 0);
        assert(points->n_points() == values->n_points());
        assert(points->n_dim() == values->n_dim());
        checked = 1;
    } // */


    // Find index of closest point
    int n;
    bool found = points->find_ann_index(point, &n);

    if (found && (0 <= n) && (n < points->n_points()))
    {
        // Retrieve corresponding value
        value = (*values)[n];
        return true;
    }

    return false;
}

// ---------------------------------------------------------------------------
