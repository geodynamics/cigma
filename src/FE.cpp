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
}

// ---------------------------------------------------------------------------

void cigma::FE::set_quadrature(Quadrature *quadrature)
{
    assert(quadrature != 0);
    this->quadrature = quadrature;
}


// ---------------------------------------------------------------------------

