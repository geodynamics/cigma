#include "Points.h"

// ---------------------------------------------------------------------------

cigma::Points::Points()
{
    num = 0;
    dim = 0;
    data = 0;
}

cigma::Points::~Points()
{
}

// ---------------------------------------------------------------------------


void cigma::Points::set_data(double *data, int num, int dim)
{
    this->data = data;
    this->num = num;
    this->dim = dim;
}

// ---------------------------------------------------------------------------
