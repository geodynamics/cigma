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
    void set_data(Cell *cell, double *quadpts, double *quadwts, int npts);

public:
    void apply_refmap();

public:
    int n_refdim() const;
    int n_globaldim() const;

public:
    int global_index(int i, int j) const;
    double global_value(int i, int j) const;

public:
    Cell *cell;
    int qdim;
    double *qpts;
    double *qwts;
};

// ---------------------------------------------------------------------------


inline int cigma::Quadrature::n_refdim() const
{
    return dim;
}

inline int cigma::Quadrature::n_globaldim() const
{
    return qdim;
}

inline int cigma::Quadrature::global_index(int i, int j) const
{
    return qdim*i + j;
}

inline double cigma::Quadrature::global_value(int i, int j) const
{
    return qpts[global_index(i,j)];
}

// ---------------------------------------------------------------------------

#endif
