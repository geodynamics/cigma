#include <cassert>
#include "FE.h"

// ---------------------------------------------------------------------------

cigma::FE::FE()
{
    quadrature = 0;
    cell = 0;

    jxw = 0;
    basis_tab = 0;
    basis_jet = 0;
}

cigma::FE::~FE()
{
    if (jxw != 0) delete [] jxw;
    if (basis_tab != 0) delete [] basis_tab;
    if (basis_jet != 0) delete [] basis_jet;
}

// ---------------------------------------------------------------------------

void cigma::FE::set_cell_quadrature(Cell *cell, Quadrature *quadrature)
{
    assert(cell != 0);
    assert(quadrature != 0);

    this->cell = cell;
    this->quadrature = quadrature;

    assert(quadrature->n_points() > 0);
    assert(quadrature->n_dim() == cell->n_celldim());

    int nq = quadrature->n_points();
    int ndofs = cell->n_nodes();
    int dim = cell->n_celldim();

    jxw = new double[nq];
    basis_tab = new double[nq * ndofs];
    basis_jet = new double[nq * ndofs * dim];
}

// ---------------------------------------------------------------------------

