#ifndef __OkadaBenchmarkFields_h__
#define __OkadaBenchmarkFields_h__

#include "Field.h"

namespace okada
{
    namespace strike_slip
    {
        class Disloc3d;
    }
    namespace reverse_slip
    {
        class Disloc3d;
    }
}

class okada::strike_slip::Disloc3d : public cigma::Field
{
public:
    Disloc3d();
    ~Disloc3d();

public:
    FieldType getType() { return USER_FIELD; }
    int n_dim() { return 3; }
    int n_rank() { return 3; }

public:
    bool eval(double *station, double *disloc);
};

class okada::reverse_slip::Disloc3d : public cigma::Field
{
public:
    Disloc3d();
    ~Disloc3d();

public:
    FieldType getType() { return USER_FIELD; }
    int n_dim() { return 3; }
    int n_rank() { return 3; }

public:
    bool eval(double *station, double *disloc);
};

#endif
