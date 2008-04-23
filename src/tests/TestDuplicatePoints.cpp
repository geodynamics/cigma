#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <utility>
#include <cassert>

#include "../PointsReader.h"

using namespace std;
using namespace cigma;

double norm2(double *x)
{
    return x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
}

double dist2(double *x, double *y)
{
    double rho[3] = {x[0]-y[0], x[1]-y[1], x[2]-y[2]};
    return norm2(rho);
}

int main()
{
    PointsReader pointsReader;
    //pointsReader.pointsPath = "../tet4_1000m.h5:/projection/points";
    pointsReader.pointsPath = "./strikeslip_tet4_1000m_t0.vtk:displacements_t0";
    pointsReader.load_points();

    Points *points = pointsReader.points;
    if (points == 0)
    {
        cerr << "Could not load points!" << endl;
        exit(1);
    }

    int npts = points->n_points();
    int dim = points->n_dim();
    assert(dim == 3);

    int a,b;
    const double epsilon2 = 1e-12;
    for (a = 0; a < npts; a++)
    {
        double *x = &(points->data[3*a]);
        for (b = 0; b < npts; b++)
        {
            if (b >= a)
            {
                break;
            }

            double *y = &(points->data[3*b]);
            double r2 = dist2(x,y);
            if (r2 < epsilon2)
            {
                cout << "(" << a << "," << b << ") ";
            }
        }
    }
    cout << endl;

    return 0;
}
