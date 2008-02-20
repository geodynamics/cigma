#include <cassert>
#include "FE.h"

// ---------------------------------------------------------------------------

cigma::FE::FE()
{
    basis_tab = 0;
    basis_jet = 0;
}

cigma::FE::~FE()
{
    if (basis_tab != 0) delete [] basis_tab;
    if (basis_jet != 0) delete [] basis_jet;
}

// ---------------------------------------------------------------------------

void cigma::FE::set_quadrature_points(QuadraturePoints *pts)
{
    assert(meshPart != 0);
    assert(meshPart->cell != 0);

    this->QuadratureRule::set_quadrature_points(pts);

    assert(points != 0);
    const int nq = points->n_points();
    const int ndofs = meshPart->cell->n_nodes();

    // get shape function values at known quadrature points
    basis_tab = new double[nq * ndofs];
    meshPart->cell->shape(nq, points->qpts, basis_tab);

    // get shape function values at known quadrature points
    //basis_jet = new double[nq * ndofs * dim];
    //meshPart->cell->grad_shape(nq, quadrature->qpts, basis_jet);
}

// ---------------------------------------------------------------------------
