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

void cigma::FE::set_cell(Cell *cell)
{
    assert(cell != 0);
    this->cell = cell;
}

void cigma::FE::set_quadrature(QuadraturePoints *quadrature)
{
    assert(cell != 0);

    this->quadrature = quadrature;

    assert(quadrature->n_points() > 0);
    assert(quadrature->n_dim() == cell->n_celldim());

    int nq = quadrature->n_points();
    int ndofs = cell->n_nodes();
    int dim = cell->n_celldim();

    jxw = new double[nq];

    // get shape function values at known quadrature points
    basis_tab = new double[nq * ndofs];
    cell->shape(nq, quadrature->qpts, basis_tab);

    // get shape function values at known quadrature points
    //basis_jet = new double[nq * ndofs * dim];
    //cell->grad_shape(nq, quadrature->qpts, basis_jet);
}

// ---------------------------------------------------------------------------

void cigma::FE::update_jxw()
{
    const int nq = quadrature->n_points();
    const int celldim = cell->n_celldim();
    for (int q = 0; q < nq; q++)
    {
        double jac[3][3];
        jxw[q] = quadrature->weight(q) * cell->jacobian((*quadrature)[q], jac);
    }
}

// ---------------------------------------------------------------------------

