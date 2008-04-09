#ifndef __OkadaBenchmarkFields_h__
#define __OkadaBenchmarkFields_h__

#include "UserField.h"

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

class okada::strike_slip::Disloc3d : public cigma::UserField
{
public:
    Disloc3d();
    ~Disloc3d();

public:
    int n_dim() { return 3; }
    int n_rank() { return 3; }

public:
    bool eval(double *station, double *disloc);

public:
    double mu, nu;
    double displacement[3];
    double derivatives[9];
    double stress[6];

    int nsubfaults;
    double *subfaults;
    double *models;
    double *flag;
    double *flag2;
};


/*
class okada::reverse_slip::Disloc3d : public cigma::UserField
{
public:
    Disloc3d();
    ~Disloc3d();

public:
    int n_dim() { return 3; }
    int n_rank() { return 3; }

public:
    bool eval(double *station, double *disloc);
};
*/

#endif
