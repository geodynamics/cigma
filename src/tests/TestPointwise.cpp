#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>

#include "../OkadaBenchmarkFields.h"
#include "../FieldReader.h"
#include "../PointField.h"

using namespace std;
using namespace cigma;


double magnitude(double *y)
{
    return sqrt(y[0]*y[0] + y[1]*y[1] + y[2]*y[2]);
}

void print_point(double *y)
{
    cout << y[0] << " " << y[1] << " " << y[2] << "   ";
}

int main()
{
    int i,j;
    int N0 = 1200;
    int N = 1000;


    FieldReader fieldReader;
    fieldReader.pointsReader.pointsPath = "../tet4_1000m.h5:/projection/points";
    fieldReader.valuesReader.pointsPath = "../tet4_1000m.h5:/projection/data/snapshot0/displacements";
    fieldReader.load_field();

    Field *field = fieldReader.field;
    if (field == 0)
    {
        cerr << "Could not load field!" << endl;
        exit(1);
    }
    if (field->getType() != Field::POINT_FIELD)
    {
        cerr << "Expecting a point field" << endl;
        exit(1);
    }
    PointField *pointField = static_cast<PointField*>(field);
    Points *points = pointField->points;
    Points *values = pointField->values;

    int npts = points->n_points();
    assert(npts > N);

    double *x = points->data;

    double *y[2];
    y[0] = new double[N*3];
    y[1] = new double[N*3];

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < 3; j++)
        {
            y[0][3*i + j] = 0.0;
            y[1][3*i + j] = 0.0;
        }
    }

    
    typedef okada::strike_slip::Disloc3d StrikeSlipDislocation;
    StrikeSlipDislocation disloc3d;
    for (i = 0; i < N; i++)
    {
        double *station = &x[3*i];
        double *displacement = &y[0][3*i];
        disloc3d.eval(station, displacement);
    }

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < 3; j++)
        {
            y[1][3*i + j] = values->data[3*(i + N0) + j];
        }
    }

    double *residuals = new double[N*3];
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < 3; j++)
        {
            residuals[3*i + j] = y[1][3*i + j] - y[0][3*i + j];
        }
    }

    double epsilon = 0.0;
    cout << setprecision(12);
    for (i = 0; i < N; i++)
    {
        cout << i << "   ";
        double *X = &x[3*i];
        double *Y0 = &y[0][3*i];
        double *Y1 = &y[1][3*i];
        double *D = &residuals[3*i];
        double eps = magnitude(D);
        epsilon += eps * eps;

        print_point(X);
        print_point(Y0);
        print_point(Y1);
        print_point(D);

        cout << magnitude(Y0) << " ";
        cout << magnitude(Y1) << " ";
        cout << eps << " ";
        cout << endl;
    }

    cerr << setprecision(12);
    cerr << "epsilon = " << sqrt(epsilon) << endl;


    delete [] x;
    delete [] y[0];
    delete [] y[1];
    delete [] residuals;

    return 0;
}
