#include "TestBenchmarkFields.h"
#include <cmath>

using namespace cigma::test_benchmarks;


// ---------------------------------------------------------------------------

Cube::Cube() {}
Cube::~Cube() {}

bool Cube::eval(double *x, double *value)
{
    value[0] = 3 * sin(M_PI * x[0]) * sin(M_PI * x[1]) * sin(M_PI * x[2]);
    return true;
}

Sphere::Sphere() {}
Sphere::~Sphere() {}

bool Sphere::eval(double *x, double *value)
{
    const double xc = 0.0;
    const double yc = 0.0;
    const double zc = 0.0;
    double dx = x[0] - xc;
    double dy = x[1] - yc;
    double dz = x[2] - zc;
    double r2 = dx*dx + dy*dy + dz*dz;
    value[0] = dx;
    value[1] = dy;
    value[2] = dz;
    return true;
}



// ---------------------------------------------------------------------------

Square::Square() {}
Square::~Square() {}

bool Square::eval(double *x, double *value)
{
    value[0] = 2 * sin(M_PI * x[0]) * sin(M_PI * x[1]);
    return true;
}


Circle::Circle() {}
Circle::~Circle() {}

bool Circle::eval(double *x, double *value)
{
    const double R2 = 0.1;
    const double xc = 0.5;
    const double yc = 0.5;
    double dx = x[0] - xc;
    double dy = x[1] - yc;
    double r2 = dx*dx + dy*dy;

    if (r2 <= R2)
    {
        value[0] = dx/R2;
        value[1] = dy/R2;
    }
    value[0] = dx/r2;
    value[1] = dy/r2;
    return true;
}

// ---------------------------------------------------------------------------
