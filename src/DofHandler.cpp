#include "DofHandler.h"
#include <cstdlib>
#include <cassert>

//----------------------------------------------------------------------------

cigma::DofHandler::DofHandler()
{
    nno = 0;
    ndim = 0;
    dofs = NULL;
}

cigma::DofHandler::~DofHandler()
{
}

//----------------------------------------------------------------------------

void cigma::DofHandler::set_data(double *dofs, int nno, int ndim)
{
    assert(dofs != NULL);
    assert(nno > 0);
    assert(ndim > 0);

    this->dofs = dofs;
    this->nno = nno;
    this->ndim = ndim;
}

void cigma::DofHandler::get_data(int num, int *nodes, double *d)
{
    for (int i = 0; i < num; i++)
    {
        int n = nodes[i];
        assert(n >= 0);
        assert(n < nno);

        for (int j = 0; j < ndim; j++)
            d[ndim*i + j] = dofs[ndim*n + j];
    }
}

