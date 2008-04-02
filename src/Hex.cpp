//#include <iostream>
#include <cmath>
#include "Hex.h"

using namespace cigma;

// ---------------------------------------------------------------------------

static void hex_shape(double u, double v, double w, double s[8])
{
    s[0] = (1.0 - u) * (1.0 - v) * (1.0 - w) * 0.125;
    s[1] = (1.0 + u) * (1.0 - v) * (1.0 - w) * 0.125;
    s[2] = (1.0 + u) * (1.0 + v) * (1.0 - w) * 0.125;
    s[3] = (1.0 - u) * (1.0 + v) * (1.0 - w) * 0.125;
    s[4] = (1.0 - u) * (1.0 - v) * (1.0 + w) * 0.125;
    s[5] = (1.0 + u) * (1.0 - v) * (1.0 + w) * 0.125;
    s[6] = (1.0 + u) * (1.0 + v) * (1.0 + w) * 0.125;
    s[7] = (1.0 - u) * (1.0 + v) * (1.0 + w) * 0.125;
}

static void hex_grad_shape(double u, double v, double w, double s[8*3])
{
    #define S(i,j) s[3*(i) + (j)]

    S(0,0) = -0.125 * (1.0 - v) * (1.0 - w);
    S(0,1) = -0.125 * (1.0 - u) * (1.0 - w);
    S(0,2) = -0.125 * (1.0 - u) * (1.0 - v);

    S(1,0) =  0.125 * (1.0 - v) * (1.0 - w);
    S(1,1) = -0.125 * (1.0 + u) * (1.0 - w);
    S(1,2) = -0.125 * (1.0 + u) * (1.0 - v);

    S(2,0) =  0.125 * (1.0 + v) * (1.0 - w);
    S(2,1) =  0.125 * (1.0 + u) * (1.0 - w);
    S(2,2) = -0.125 * (1.0 + u) * (1.0 + v);

    S(3,0) = -0.125 * (1.0 + v) * (1.0 - w);
    S(3,1) =  0.125 * (1.0 - u) * (1.0 - w);
    S(3,2) = -0.125 * (1.0 - u) * (1.0 + v);

    S(4,0) = -0.125 * (1.0 - v) * (1.0 + w);
    S(4,1) = -0.125 * (1.0 - u) * (1.0 + w);
    S(4,2) =  0.125 * (1.0 - u) * (1.0 - v);

    S(5,0) =  0.125 * (1.0 - v) * (1.0 + w);
    S(5,1) = -0.125 * (1.0 + u) * (1.0 + w);
    S(5,2) =  0.125 * (1.0 + u) * (1.0 - v);

    S(6,0) =  0.125 * (1.0 + v) * (1.0 + w);
    S(6,1) =  0.125 * (1.0 + u) * (1.0 + w);
    S(6,2) =  0.125 * (1.0 + u) * (1.0 + v);

    S(7,0) = -0.125 * (1.0 + v) * (1.0 + w);
    S(7,1) =  0.125 * (1.0 - u) * (1.0 + w);
    S(7,2) =  0.125 * (1.0 - u) * (1.0 + v);

    #undef S
}


// ---------------------------------------------------------------------------

Hex::Hex()
{
    const int hex_nno = 8;
    const int hex_celldim = 3;
    double verts[hex_nno * hex_celldim] = {
        -1.0, -1.0, -1.0,
        +1.0, -1.0, -1.0,
        +1.0, +1.0, -1.0,
        -1.0, +1.0, -1.0,
        -1.0, -1.0, +1.0,
        +1.0, -1.0, +1.0,
        +1.0, +1.0, +1.0,
        -1.0, +1.0, +1.0
    };
    set_reference_vertices(verts, hex_nno);
}

Hex::~Hex()
{
}


void Hex::qr_default(double **wts, double **pts, int *npts, int *ndim)
{
    // hex_qr(3)
    const int hex_nno = 8;
    const int hex_celldim = 3;
    double hex_qpts[hex_nno * hex_celldim] = {
        -0.57735027, -0.57735027, -0.57735027,
         0.57735027, -0.57735027, -0.57735027,
         0.57735027,  0.57735027, -0.57735027,
        -0.57735027,  0.57735027, -0.57735027,
        -0.57735027, -0.57735027,  0.57735027,
         0.57735027, -0.57735027,  0.57735027,
         0.57735027,  0.57735027,  0.57735027,
        -0.57735027,  0.57735027,  0.57735027
    };
    double hex_qwts[hex_nno] = { 1.,  1.,  1.,  1.,  1.,  1.,  1.,  1. };

    int i,j,n;
    *npts = hex_nno;
    *ndim = hex_celldim;
    *wts = new double[(*npts)];
    *pts = new double[(*npts) * (*ndim)];
    for (i = 0; i < hex_nno; i++)
    {
        (*wts)[i] = hex_qwts[i];
        for (j = 0; j < (*ndim); j++)
        {
            n = (*ndim) * i + j;
            (*pts)[n] = hex_qpts[n];
        }
    }
}


// ---------------------------------------------------------------------------

/*
 * @param points points is an [num x celldim] array (in)
 * @param values values is an [num x ndofs] array (out)
 *
 */
void Hex::shape(int num, double *points, double *values)
{
    const int nno = n_nodes();
    for (int i = 0; i < num; i++)
    {
        double u = points[3*i + 0];
        double v = points[3*i + 1];
        double w = points[3*i + 2];
        hex_shape(u, v, w, &values[nno*i]);
    }
}

/*
 *
 * @param points points is an [num x celldim] array (in)
 * @param values values is an [num x ndofs x celldim] array (out)
 */
void Hex::grad_shape(int num, double *points, double *values)
{
    const int nno = n_nodes();
    const int celldim = n_celldim();
    const int stride = nno * celldim;
    for (int i = 0; i < num; i++)
    {
        double u = points[3*i + 0];
        double v = points[3*i + 1];
        double w = points[3*i + 2];
        hex_grad_shape(u, v, w, &values[stride*i]);
    }
}

/*
 *
 */
#define ZERO    (-1.0e-6)
#define ONE     (1-ZERO)

bool Hex::interior(double u, double v, double w)
{

    if ((u < -ONE) || (v < -ONE) || (w < -ONE) ||
        (u > +ONE) || (v > +ONE) || (w > +ONE))
    {
        return false;
    }
    return true;

}

#undef ONE
#undef ZERO


double Hex::volume()
{
    //using namespace std;


    /*
    // 
    // 6V = [(x7-x0),(x1-x0),(x3-x5)]
    //    + [(x7-x0),(x4-x0),(x5-x6)]
    //    + [(x7-x0),(x2-x0),(x6-x3)]
    // 
    // where
    //
    //  xn = (globverts[3*n+0],globverts[3*n+1],globverts[3*n+2])
    //
    //            | A_x B_x C_x |
    //  [A,B,C] = | A_y B_y C_y |
    //            | A_z B_z C_z |
    //
    double x70[3],x10[3],x35[3],x40[3],x56[3],x20[3],x63[3];
    #define X(j)    globverts[3*(j)+i]
    for (int i = 0; i < 3; i++)
    {
        x70[i] = X(7) - X(0);
        x10[i] = X(1) - X(0);
        x35[i] = X(3) - X(5);
        x40[i] = X(4) - X(0);
        x56[i] = X(5) - X(6);
        x20[i] = X(2) - X(0);
        x63[i] = X(6) - X(3);
    }
    #undef X
    return (
          x70[0] * x10[1] * x35[2]
        - x70[0] * x35[1] * x10[2] 
        - x70[1] * x10[0] * x35[2] 
        + x70[1] * x35[0] * x10[2] 
        + x70[2] * x10[0] * x35[1] 
        - x70[2] * x35[0] * x10[1] 
        + x70[0] * x40[1] * x56[2] 
        - x70[0] * x56[1] * x40[2] 
        - x70[1] * x40[0] * x56[2] 
        + x70[1] * x56[0] * x40[2] 
        + x70[2] * x40[0] * x56[1] 
        - x70[2] * x56[0] * x40[1] 
        + x70[0] * x20[1] * x63[2] 
        - x70[0] * x63[1] * x20[2] 
        - x70[1] * x20[0] * x63[2] 
        + x70[1] * x63[0] * x20[2] 
        + x70[2] * x20[0] * x63[1] 
        - x70[2] * x63[0] * x20[1]
    ) / 6.0;*/


    /*
    #define X(n) globverts[3*(n) + i]
    #define TRIPLE (A[0] * B[1] * C[2] - A[0] * C[1] * B[2] - A[1] * B[0] * C[2] + A[1] * C[0] * B[2] + A[2] * B[0] * C[1] - A[2] * C[0] * B[1])

    int i;
    double vol = 0;
    double A[3],B[3],C[3];

    vol = 0.0;

    for (i = 0; i < 3; i++)
    {
        A[i] =  (X(7) - X(0));
        B[i] =  (X(1) + X(6)) - (X(4) + X(5));
        C[i] = -(X(1) - X(6)) + (X(4) - X(5));
    }

    vol += TRIPLE;

    for (i = 0; i < 3; i++)
    {
        B[i] = (X(1) + X(6)) - (X(3) + X(2));
        C[i] = (X(1) - X(6)) + (X(3) - X(2));
    }

    vol += TRIPLE;

    return vol/12.0;

    #undef X
    #undef TRIPLE
    */


    /*
    #define X(n)   (globverts[3*(n) + i])
    #define TRIPLE (A[0] * B[1] * C[2] - A[0] * C[1] * B[2] - A[1] * B[0] * C[2] + A[1] * C[0] * B[2] + A[2] * B[0] * C[1] - A[2] * C[0] * B[1])

    int i;
    double vol = 0;
    double A[3], B[3], C[3];

    for (int n = 0; n < 8; n++){for (i = 0; i < 3; i++){std::cerr << X(n) << " ";}std::cerr << std::endl;}

    for (i = 0; i < 3; i++)
    {
        A[i] = (X(7) - X(1)) + (X(6) - X(0));
        B[i] = (X(7) - X(2));
        C[i] = (X(3) - X(0));
    }
    vol += (TRIPLE);

    for (i = 0; i < 3; i++)
    {
        A[i] = (X(6) - X(0));
        B[i] = (X(7) - X(2)) + (X(5) - X(0));
        C[i] = (X(7) - X(4));
    }
    vol += (TRIPLE);

    for (i = 0; i < 3; i++)
    {
        A[i] = (X(7) - X(1));
        B[i] = (X(5) - X(0));
        C[i] = (X(7) - X(4)) + (X(3) - X(0));
    }
    vol += (TRIPLE);

    return vol / 12.0;

    #undef TRIPLE
    #undef X
    */


    /*
     * Permutation of vertices: (2 3)(6 7)
     */
    #define X(n)   (globverts[3*(n) + i])
    #define TRIPLE (A[0] * B[1] * C[2] - A[0] * C[1] * B[2] - A[1] * B[0] * C[2] + A[1] * C[0] * B[2] + A[2] * B[0] * C[1] - A[2] * C[0] * B[1])

    int i;
    double vol = 0;
    double A[3], B[3], C[3];

    //for (int n = 0; n < 8; n++){for (i = 0; i < 3; i++){std::cerr << X(n) << " ";}std::cerr << std::endl;}

    for (i = 0; i < 3; i++)
    {
        A[i] = (X(6) - X(1)) + (X(7) - X(0));
        B[i] = (X(6) - X(3));
        C[i] = (X(2) - X(0));
    }
    vol += (TRIPLE);

    for (i = 0; i < 3; i++)
    {
        A[i] = (X(7) - X(0));
        B[i] = (X(6) - X(3)) + (X(5) - X(0));
        C[i] = (X(6) - X(4));
    }
    vol += (TRIPLE);

    for (i = 0; i < 3; i++)
    {
        A[i] = (X(6) - X(1));
        B[i] = (X(5) - X(0));
        C[i] = (X(6) - X(4)) + (X(2) - X(0));
    }
    vol += (TRIPLE);

    return vol / 12.0;

    #undef TRIPLE
    #undef X
}
