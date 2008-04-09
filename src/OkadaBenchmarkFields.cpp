#include "OkadaBenchmarkFields.h"
#include "okada/cervelli.h"
#include "Misc.h"

using namespace cigma;
using namespace okada;

// ---------------------------------------------------------------------------

strike_slip::Disloc3d::Disloc3d()
{
    // elastic parameters
    mu = 0.25;
    nu = 0.25;

    // allocate fault info
    nsubfaults = 400;
    subfaults = new double[nsubfaults * 11];
    models = new double[nsubfaults * 10];
    flag = new double[1];
    flag2 = new double[nsubfaults];

    //
    // define strike slip fault
    //
    int i;
    double *n;
    double taperd;

    taperd = 10.0;  // (16000.0 - 12000.0)/400
    linspace(&n, 12000.0, 16000.0, nsubfaults);

    double *fi;
    double fx1, fy1;
    double fx2, fy2;
    double dip, D, Pr, bd;
    double ss, ds, ts;

    for (i = 0; i < nsubfaults; i++)
    {
        fi = &subfaults[11*i];

        fx1 = 12000.0;
        fy1 = -n[i];

        fx2 = 12000.0;
        fy2 = n[i];

        dip = 90.0;
        D   = n[i];
        Pr  = 0.25;
        bd  = 0;
        ss  = -1.0/nsubfaults;
        ds  = 0;
        ts  = 0;

        fi[0]  = fx1;
        fi[1]  = fy1;
        fi[2]  = fx2;
        fi[3]  = fy2;
        fi[4]  = dip;
        fi[5]  = D;
        fi[6]  = Pr;
        fi[7]  = bd;
        fi[8]  = ss;
        fi[9]  = ds;
        fi[10] = ts;
    }

    delete [] n;
}

strike_slip::Disloc3d::~Disloc3d()
{
    nsubfaults = 0;
    delete [] subfaults;
    delete [] models;
    delete [] flag;
    delete [] flag2;
}

bool strike_slip::Disloc3d::eval(double *station, double *disloc)
{
    //
    // XXX: deconstruct the following call.
    // Specifically, move the call to getM() into the constructor,
    // and then, instead, call disloc3d() with the appropriate arguments
    //
    calc_disp_cervelli(
        mu, nu,
        models, subfaults, nsubfaults,
        station, 1,
        displacement, derivatives, stress,
        flag, flag2);

    //
    // copy the answer into the output array
    // XXX: add a selection method which defaults to displacements?
    //
    disloc[0] = displacement[0];
    disloc[1] = displacement[1];
    disloc[2] = displacement[2];

    if (*flag > 0)
    {
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
