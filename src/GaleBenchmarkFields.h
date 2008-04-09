#ifndef __GaleBenchmarkFields_h__
#define __GaleBenchmarkFields_h__

#include "UserField.h"

namespace gale
{
    namespace circular_inclusion
    {
        class Pressure;
    }
}

class gale::circular_inclusion::Pressure : public cigma::UserField
{
public:
    Pressure();
    ~Pressure();

public:
    int n_dim() { return 2; }
    int n_rank() { return 1; }

public:
    bool eval(double *x, double *value);
};

#endif

