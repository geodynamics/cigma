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
    void set_data(double *data, int num, int dim);
    void set_locator(Locator *locator);

public:
    double operator()(int i, int j);
    double *operator[](int i);

public:
    int n_points() const;
    int n_dim() const;
    int index(int i, int j) const;

public:
    int num;
    int dim;
    double *data;

public:
    Locator *locator;
};

// ---------------------------------------------------------------------------

inline double cigma::Points::operator()(int i, int j)
{
    //assert(0 <= i); assert(i < num);
    //assert(0 <= j); assert(j < dim);
    return data[index(i,j)];
}

inline double *cigma::Points::operator[](int i)
{
    //assert(0 <= i); assert(i < num);
    return &data[dim*i];
}

inline int cigma::Points::n_points() const
{
    return num;
}

inline int cigma::Points::n_dim() const
{
    return dim;
}

inline int cigma::Points::index(int i, int j) const
{
    //assert(0 <= i); assert(i < num);
    //assert(0 <= j); assert(j < dim);
    return dim*i + j;
}

// ---------------------------------------------------------------------------

#endif
