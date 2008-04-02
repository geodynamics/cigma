#include <cmath>

#include "Tet.h"
#include "Numeric.h"

using namespace cigma;

// ---------------------------------------------------------------------------

static void tet_shape(double u, double v, double w, double s[4])
{
    s[0] = 1.0 - u - v - w;
    s[1] =       u        ;
    s[2] =           v    ;
    s[3] =               w;
}

static void tet_grad_shape(double u, double v, double w, double s[4*3])
{
    #define S(i,j) s[3*(i) + (j)]

    S(0,0) = -1.0;
    S(0,1) = -1.0;
    S(0,2) = -1.0;
    
    S(1,0) = +1.0;
    S(1,1) =  0.0;
    S(1,2) =  0.0;

    S(2,0) =  0.0;
    S(2,1) = +1.0;
    S(2,2) =  0.0;

    S(3,0) =  0.0;
    S(3,1) =  0.0;
    S(3,2) = +1.0;

    #undef S
}


// ---------------------------------------------------------------------------

Tet::Tet()
{
    const int tet_nno = 4;
    const int tet_celldim = 3;
    double verts[tet_nno * tet_celldim] = {
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };
    set_reference_vertices(verts, tet_nno);
}

Tet::~Tet()
{
}

void Tet::qr_default(double **wts, double **pts, int *npts, int *ndim)
{
    // tet_qr(3)
    const int tet_nno = 8;
    const int tet_celldim = 3;
    double tet_qpts[tet_nno * tet_celldim] = {
        -0.68663473, -0.72789005, -0.75497035,
        -0.83720867, -0.85864055,  0.08830369,
        -0.86832263,  0.13186633, -0.75497035,
        -0.93159441, -0.4120024 ,  0.08830369,
         0.16949513, -0.72789005, -0.75497035,
        -0.39245447, -0.85864055,  0.08830369,
        -0.50857335,  0.13186633, -0.75497035,
        -0.74470688, -0.4120024 ,  0.08830369
    };
    double tet_qwts[tet_nno] = {
        0.29583885,  0.12821632,  0.16925605,  0.07335544,  0.29583885,
        0.12821632,  0.16925605,  0.07335544
    };

    int i,j,n;

    for (i = 0; i < tet_nno; i++)
    {
        for (j = 0; j < tet_celldim; j++)
        {
            n = tet_celldim * i + j;
            tet_qpts[n] += 1;
            tet_qpts[n] *= 0.5;
        }
    }

    *npts = tet_nno;
    *ndim = tet_celldim;
    *wts = new double[(*npts)];
    *pts = new double[(*npts) * (*ndim)];
    for (i = 0; i < tet_nno; i++)
    {
        (*wts)[i] = tet_qwts[i];
        for (j = 0; j < (*ndim); j++)
        {
            n = (*ndim) * i + j;
            (*pts)[n] = tet_qpts[n];
        }
    }
}

void Tet::shape(int num, double *points, double *values)
{
    const int nno = n_nodes();
    for (int i = 0; i < num; i++)
    {
        double u = points[3*i + 0];
        double v = points[3*i + 1];
        double w = points[3*i + 2];
        tet_shape(u, v, w, &values[nno*i]);
    }
}

void Tet::grad_shape(int num, double *points, double *values)
{
    const int nno = n_nodes();
    const int celldim = n_celldim();
    const int stride = nno * celldim;
    for (int i = 0; i < num; i++)
    {
        double u = points[3*i + 0];
        double v = points[3*i + 1];
        double w = points[3*i + 2];
        tet_grad_shape(u, v, w, &values[stride*i]);
    }
}

void Tet::xyz2uvw(double xyz[3], double uvw[3])
{
    double det;
    double mat[3][3], b[3];

    /*
     *  vertices = {(x_0,y_0,z_0),
     *              (x_1,y_1,z_1),
     *              (x_2,y_2,z_2),
     *              (x_3,y_3,z_3)}
     *
     *        [ (x[1]-x[0])  (x[2]-x[0])  (x[3]-x[0]) ]
     *  mat = [ (y[1]-y[0])  (y[2]-y[0])  (y[3]-y[0]) ]
     *        [ (z[1]-z[0])  (z[2]-z[0])  (z[3]-z[0]) ]
     *
     *  x[i] = globverts[3*i+0]
     *  y[i] = globverts[3*i+1]
     *  z[i] = globverts[3*i+2]
     *
     */

    #define X(i)  globverts[3*(i)+0]
    #define Y(i)  globverts[3*(i)+1]
    #define Z(i)  globverts[3*(i)+2]

    mat[0][0] = X(1) - X(0);
    mat[0][1] = X(2) - X(0);
    mat[0][2] = X(3) - X(0);

    mat[1][0] = Y(1) - Y(0);
    mat[1][1] = Y(2) - Y(0);
    mat[1][2] = Y(3) - Y(0);

    mat[2][0] = Z(1) - Z(0);
    mat[2][1] = Z(2) - Z(0);
    mat[2][2] = Z(3) - Z(0);

    b[0] = xyz[0] - X(0);
    b[1] = xyz[1] - Y(0);
    b[2] = xyz[2] - Z(0);

    #undef X
    #undef Y
    #undef Z

    sys3x3(mat, b, uvw, &det);

}

bool Tet::interior(double u, double v, double w)
{
    #define ZERO  (-1.0e-6)
    #define ONE   (1 - ZERO)

    if ((u < ZERO) || (v < ZERO) || (w < ZERO) || (u > (ONE - v - w)))
        return false;
    return true;

    #undef ONE
    #undef ZERO
}

double Tet::volume()
{
    //      
    //      | x0 y0 z0 1 |
    // 6V = | x1 y1 z1 1 |
    //      | x2 y2 z2 1 |
    //      | x3 y3 z3 1 |
    //
    double *a = globverts;
    return fabs(a[0] * a[4] * a[8]
        - a[0] * a[4] * a[11] 
        + a[0] * a[7] * a[11]
        - a[0] * a[7] * a[5] 
        + a[0] * a[10] * a[5] 
        - a[0] * a[10] * a[8] 
        - a[3] * a[1] * a[8] 
        + a[3] * a[1] * a[11] 
        - a[3] * a[7] * a[11] 
        + a[3] * a[7] * a[2] 
        - a[3] * a[10] * a[2] 
        + a[3] * a[10] * a[8] 
        + a[6] * a[1] * a[5] 
        - a[6] * a[1] * a[11] 
        + a[6] * a[4] * a[11] 
        - a[6] * a[4] * a[2] 
        + a[6] * a[10] * a[2] 
        - a[6] * a[10] * a[5] 
        - a[9] * a[1] * a[5] 
        + a[9] * a[1] * a[8] 
        - a[9] * a[4] * a[8] 
        + a[9] * a[4] * a[2] 
        - a[9] * a[7] * a[2] 
        + a[9] * a[7] * a[5]) / 6.0;
}
