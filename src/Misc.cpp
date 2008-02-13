#include <iostream>
#include <iomanip>
#include <cassert>
#include <string>

#include "Misc.h"


using namespace std;


// ---------------------------------------------------------------------------


double pick_from_interval(double a, double b)
{
    return a + (b - a) * rand()/(RAND_MAX + 1.0);
}


void bbox_random_point(double minpt[3], double maxpt[3], double x[3])
{
    const int nsd = 3;
    for (int i = 0; i < nsd; i++)
    {
        assert(minpt[i] <= maxpt[i]);
        x[i] = pick_from_interval(minpt[i], maxpt[i]);
    }
}


// ---------------------------------------------------------------------------
