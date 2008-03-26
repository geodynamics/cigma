#ifndef __POINTS_H__
#define __POINTS_H__

#include <cassert>
#include "Locator.h"

namespace cigma
{
    class Points;
}

/**
 * @brief Array object representing a set of points as a contiguous
 * memory block (row major storage).
 *
 */
class cigma::Points
{
public:
    Points();
    ~Points();

public:
    void set_data(double *data, int npts, int ndim);
    void set_locator(Locator *locator);

public:
    double operator()(int i, int j);
    double *operator[](int i);

public:
    int n_points() const;
    int n_dim() const;
    int index(int i, int j) const;

public:
    int npts;
    int ndim;
    double *data;

public:
    Locator *locator;
    bool find_ann_index(double *point, int *annIndex);
};

// ---------------------------------------------------------------------------

inline double cigma::Points::operator()(int i, int j)
{
    //assert(0 <= i); assert(i < npts);
    //assert(0 <= j); assert(j < dim);
    return data[index(i,j)];
}

inline double *cigma::Points::operator[](int i)
{
    //assert(0 <= i); assert(i < npts);
    return &data[ndim*i];
}

inline int cigma::Points::n_points() const
{
    return npts;
}

inline int cigma::Points::n_dim() const
{
    return ndim;
}

inline int cigma::Points::index(int i, int j) const
{
    //assert(0 <= i); assert(i < npts);
    //assert(0 <= j); assert(j < ndim);
    return ndim*i + j;
}

// ---------------------------------------------------------------------------

#endif
