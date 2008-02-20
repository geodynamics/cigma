#ifndef __QUADRATURE_POINTS_H__
#define __QUADRATURE_POINTS_H__

#include "Cell.h"
#include "Points.h"

namespace cigma
{
    class QuadraturePoints;
}

/**
 * @brief QuadraturePoints scheme
 *
 */

class cigma::QuadraturePoints : public Points
{
public:
    QuadraturePoints();
    ~QuadraturePoints();

public:
    void set_quadrature(double *quadpts, double *quadwts, int npts, int qdim);
    void set_globaldim(int dim);
    void apply_refmap(Cell *cell);

public:
    int n_refdim() const;
    int n_globaldim() const;

public:
    double point(int i, int j) const;
    double weight(int i) const;

public:
    int qdim;
    double *qpts;
    double *qwts;
};

// ---------------------------------------------------------------------------


inline int cigma::QuadraturePoints::n_refdim() const
{
    return qdim;
}

inline int cigma::QuadraturePoints::n_globaldim() const
{
    return dim;
}

inline double cigma::QuadraturePoints::point(int i, int j) const
{
    return qpts[qdim*i + j];
}

inline double cigma::QuadraturePoints::weight(int i) const
{
    return qwts[i];
}

// ---------------------------------------------------------------------------

#endif
