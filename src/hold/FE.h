#ifndef __FE_H__
#define __FE_H__

#include "Cell.h"
#include "Points.h"
#include "QuadraturePoints.h"
#include "QuadratureRule.h"

namespace cigma
{
    class FE;   // XXX: rename to FE_QR since now we derive from QuadratureRule
}

/**
 * @brief Tabulation for Finite Elements;
 *
 */
class cigma::FE : public QuadratureRule
{
public:
    FE();
    ~FE();

public:
    void set_quadrature_points(QuadraturePoints *pts);

public:
    double *basis_tab;  // [nq x ndofs]
    double *basis_jet;  // [nq x ndofs x celldim]
};

#endif
