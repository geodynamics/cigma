#ifndef __POINT_FIELD_H__
#define __POINT_FIELD_H__

#include "Field.h"
#include "Points.h"

namespace cigma
{
    class PointField;
}

/**
 * @brief Point field object
 *
 */

class cigma::PointField : public Field
{
public:
    PointField();
    ~PointField();

public:
    void set_points(double *pts, int npts, int nsd);
    void set_values(double *vals, int nvals, int rank);
    void set_locator(Locator *locator);

public:
    void eval(double *point, double *value);

public:
    Points *points;
    Points *values;

public:
    Locator *locator;
};


#endif
