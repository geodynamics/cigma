#include <cassert>
#include "Field.h"

cigma::Field::~Field() {};

void cigma::Field::eval(Points &domain, Points &range)
{
    assert(domain.n_points() == range.n_points());
    for (int i = 0; i < domain.n_points(); i++)
    {
        double *x = domain[i];
        double *y = range[i];
        eval(x,y);
    }
}

