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

