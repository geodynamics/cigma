#include <cmath>
#include <iostream>
#include <typeinfo>
#include "GaleBenchmarkFields.h"

using namespace gale::circular_inclusion;

Pressure::Pressure()
{
    //std::cout << "Creating instance of class " << typeid(this).name() << std::endl;
}

Pressure::~Pressure()
{
}

bool Pressure::eval(double *x, double *value)
{
    const double R = 0.05;
    const double C = (8*9/11.0)*R*R;
    const double xc = 0.5;
    const double yc = 0.5;
    double dx = x[0] - xc;
    double dy = x[1] - yc;
    double r2 = dx*dx + dy*dy;
    if (r2 < R*R)
    {
        value[0] = 4.0;
        return true;
    }

    double theta = atan2(dy, dx);
    value[0] = (C/r2) * cos(2*theta);
    return true;
}

