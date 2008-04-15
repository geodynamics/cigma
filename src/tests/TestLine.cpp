#include <iostream>
#include <iomanip>
#include <cmath>

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
    int N = 400;
    double A[3] = {     0, 18000, -12000 };
    double B[3] = { 24000, 18000, -12000 };

    int i,j;
    double t, dt;
    dt = 1.0 / (N - 1);

    double *x = new double[N*3];
    for (i = 0; i < N; i++)
    {
        t = i * dt;
        for (j = 0; j < 3; j++)
        {
            x[3*i + j] = A[j] + t * (B[j] - A[j]);
        }
    }

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
    //PointField *pointField = static_cast<PointField*>(fieldReader.field);
    for (i = 0; i < N; i++)
    {
        double *station = &x[3*i];
        double *displacement = &y[1][3*i];
        field->eval(station, displacement);
    }


    double *residuals = new double[N*3];
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < 3; j++)
        {
            residuals[3*i + j] = y[1][3*i + j] - y[0][3*i + j];
        }
    }

    cout << setprecision(12);
    for (i = 0; i < N; i++)
    {
        t = i * dt;
        cout << i << " ";
        cout << t << "   ";

        double *X = &x[3*i];
        double *Y0 = &y[0][3*i];
        double *Y1 = &y[1][3*i];
        double *D = &residuals[3*i];

        print_point(X);
        print_point(Y0);
        print_point(Y1);
        print_point(D);

        cout << magnitude(Y0) << " ";
        cout << magnitude(Y1) << " ";
        cout << magnitude(D) << " ";
        cout << endl;
    }


    delete [] x;
    delete [] y[0];
    delete [] y[1];
    delete [] residuals;

    return 0;
}
