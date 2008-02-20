#ifndef __FE_H__
#define __FE_H__

#include "Cell.h"
#include "Points.h"
#include "QuadraturePoints.h"

namespace cigma
{
    class FE;
}

/**
 * @brief Tabulation for Finite Elements;
 *
 */
class cigma::FE
{
public:
    FE();
    ~FE();

public:
    void set_cell(Cell *cell);
    void set_quadrature(QuadraturePoints *quadrature);

public:
    void update_jxw();

public:
    Cell *cell;
    QuadraturePoints *quadrature;
    double *jxw;
    double *basis_tab;  // [nq x ndofs]
    double *basis_jet;  // [nq x ndofs x celldim]
};

#endif
