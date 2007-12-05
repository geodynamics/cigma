#include "Numeric.h"

using namespace cigma;

void cigma::prodve(double a[3], double b[3], double c[3])
{
    c[2] =  a[0] * b[1] - a[1] * b[0];
    c[1] = -a[0] * b[2] + a[2] * b[0];
    c[0] =  a[1] * b[2] - a[2] * b[1];
}

void cigma::matvec(double mat[3][3], double vec[3], double res[3])
{
    res[0] = mat[0][0] * vec[0] + mat[0][1] * vec[1] + mat[0][2] * vec[2];
    res[1] = mat[1][0] * vec[0] + mat[1][1] * vec[1] + mat[1][2] * vec[2];
    res[2] = mat[2][0] * vec[0] + mat[2][1] * vec[1] + mat[2][2] * vec[2];
}

double cigma::det3x3(double mat[3][3])
{
    return (mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) -
            mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
            mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]));
}

double cigma::inv3x3(double mat[3][3], double inv[3][3])
{
    double det = det3x3(mat);
    if (det)
    {
        double ud = 1.0 / det;
        inv[0][0] =  ( mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1] ) * ud ;
        inv[1][0] = -( mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0] ) * ud ;
        inv[2][0] =  ( mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0] ) * ud ;
        inv[0][1] = -( mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1] ) * ud ;
        inv[1][1] =  ( mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0] ) * ud ;
        inv[2][1] = -( mat[0][0] * mat[2][1] - mat[0][1] * mat[2][0] ) * ud ;
        inv[0][2] =  ( mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1] ) * ud ;
        inv[1][2] = -( mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0] ) * ud ;
        inv[2][2] =  ( mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0] ) * ud ;
    }
    else
    {
        // journal(error, "singular matrix");
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                inv[i][j] = 0.0;
    }

    return det;
}

int cigma::sys3x3(double mat[3][3], double b[3], double res[3], double *det)
{
    double ud;
    int i;

    *det = det3x3(mat);

    if (*det == 0.0)
    {
        res[0] = res[1] = res[2] = 0.0;
        return 0;
    }

    ud = 1.0 / (*det);

    res[0] = b[0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
             -  mat[0][1] * (b[1] * mat[2][2] - mat[1][2] * b[2])
             +  mat[0][2] * (b[1] * mat[2][1] - mat[1][1] * b[2]);

    res[1] = mat[0][0] * (b[1] * mat[2][2] - mat[1][2] * b[2])
             - b[0] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
             + mat[0][2] * (mat[1][0] * b[2] - b[1] * mat[2][0]);

    res[2] = mat[0][0] * (mat[1][1] * b[2] - b[1] * mat[2][1])
             - mat[0][1] * (mat[1][0] * b[2] - b[1] * mat[2][0])
             + b[0] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);

    for (i = 0; i < 3; i++)
    {
        res[i] *= ud;
    }

    return 1;
}

void cigma::minmax(int n, double *X, double *Y, double *Z, double min[3], double max[3])
{
    min[0] = X[0];
    min[1] = Y[0];
    min[2] = Z[0];

    max[0] = X[0];
    max[1] = Y[0];
    max[2] = Z[0];

    for (int i = 1; i < n; i++)
    {
        min[0] = MIN(X[i], min[0]);
        min[1] = MIN(Y[i], min[1]);
        min[2] = MIN(Z[i], min[2]);

        max[0] = MAX(X[i], max[0]);
        max[1] = MAX(Y[i], max[1]);
        max[2] = MAX(Z[i], max[2]);
    }
}

void cigma::centroid(int n, double *X, double *Y, double *Z, double c[3])
{
    const double oc = 1.0 / (double)n;

    c[0] = X[0];
    c[1] = Y[0];
    c[2] = Z[0];

    for (int i = 1; i < n; i++)
    {
        c[0] += X[i];
        c[1] += Y[i];
        c[2] += Z[i];
    }

    c[0] *= oc;
    c[1] *= oc;
    c[2] *= oc;
}

