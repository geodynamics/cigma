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

void cigma::minmax(double *points, int npts, int nsd, double *min, double *max)
{
    // 
    // X[i] = points[nsd*i + 0]
    // Y[i] = points[nsd*i + 1]
    // Z[i] = points[nsd*i + 2]
    //
    // PT_n = (X[n],Y[n],Z[n])  (n-th point)
    //
    
    int i,j;

    #define PT(i,j) points[nsd*(i) + (j)]

    for (j = 0; j < nsd; j++)
    {
        min[j] = PT(0,j);
        max[j] = PT(0,j);
    }

    for (i = 1; i < npts; i++)
    {
        for (j = 0; j < nsd; j++)
        {
            min[j] = MIN(PT(i,j), min[j]);
            max[j] = MAX(PT(i,j), max[j]);
        }
    }

    #undef PT
}

void cigma::centroid(double *points, int npts, int nsd, double c[3])
{
    int i, j;
    const double oc = 1.0 / (double)npts;

    #define PT(i,j)  points[nsd*(i) + (j)]

    c[0] = c[1] = c[2] = 0.0;
    for (j = 0; j < nsd; j++)
    {
        c[j] = PT(0,j);
    }

    for (i = 1; i < npts; i++)
    {
        for (j = 0; j < nsd; j++)
            c[j] += PT(i,j);
    }

    for (j = 0; j < nsd; j++)
    {
        c[j] *= oc;
    }

    #undef PT

}

