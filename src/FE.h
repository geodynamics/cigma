#ifndef __FE_H__
#define __FE_H__

#include "Cell.h"
#include "Points.h"
#include "Quadrature.h"

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
    void set_cell_quadrature(Cell *cell, Quadrature *quadrature);

public:
    Cell *cell;
    Quadrature *quadrature;
    double *jxw;
    double *basis_tab;  // [nq x ndofs]
    double *basis_jet;  // [nq x ndofs x celldim]
};

#endif
