#include <iostream>
#include <cassert>
#include "PointField.h"

using namespace std;
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
    const bool debug = false;

    // XXX: sanity check
    //static int checked = 0;
    //if (!checked)
    //{
    //    assert(points->n_points() != 0);
    //    assert(values->n_points() != 0);
    //    assert(points->n_points() == values->n_points());
    //    assert(points->n_dim() == values->n_dim());
    //    checked = 1;
    //}

    // Find index of closest point
    int n;
    bool found = points->find_ann_index(point, &n);
    if (!found) { return false; }

    if ((0 <= n) && (n < points->n_points()))
    {
        int i;

        // Show ANN point
        if (debug)
        {
            double *p = (*points)[n];

            cerr << "ANNPT( ";
            for (i = 0; i < points->n_dim(); i++)
            {
                cerr << p[i] << " ";
            }
            cerr << ")" << endl;
        }

        // Retrieve corresponding value
        double *v = (*values)[n];
        for (i = 0; i < values->n_dim(); i++)
        {
            value[i] = v[i];
        }
        return true;
    }

    return false;
}

// ---------------------------------------------------------------------------
