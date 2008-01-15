#ifndef __QUADRATURE_H__
#define __QUADRATURE_H__

#include "Cell.h"
#include "Points.h"

namespace cigma
{
    class Quadrature;
}

/**
 * @brief Quadrature scheme
 *
 */

class cigma::Quadrature : public Points
{
public:
    Quadrature();
    ~Quadrature();

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


inline int cigma::Quadrature::n_refdim() const
{
    return qdim;
}

inline int cigma::Quadrature::n_globaldim() const
{
    return dim;
}

inline double cigma::Quadrature::point(int i, int j) const
{
    return qpts[qdim*i + j];
}

inline double cigma::Quadrature::weight(int i) const
{
    return qwts[i];
}

// ---------------------------------------------------------------------------

#endif
