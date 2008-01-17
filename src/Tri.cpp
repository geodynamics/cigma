#include "Tri.h"
#include <cassert>

// ---------------------------------------------------------------------------

static void tri_shape(double u, double v, double s[3])
{
    s[0] = 1.0 - u - v ;
    s[1] =       u     ;
    s[2] =           v ;
}

static void tri_grad_shape(double u, double v, double s[3*2])
{
    #define S(i,j) s[2*(i) + (j)]

    S(0,0) = -1.0;
    S(0,1) = -1.0;

    S(1,0) = +1.0;
    S(1,1) =  0.0;

    S(2,0) =  0.0;
    S(2,1) = +1.0;

    #undef S
}


// ---------------------------------------------------------------------------

cigma::Tri::Tri()
{
    const int tri_nno = 3;
    const int tri_celldim = 2; // XXX
    double verts[tri_nno * tri_celldim] = {
        0.0, 0.0,
        1.0, 0.0,
        0.0, 1.0
    };
    set_reference_vertices(verts, tri_nno);
}

cigma::Tri::~Tri()
{
}

// ---------------------------------------------------------------------------

void cigma::Tri::shape(int num, double *points, double *values)
{
    const int nno = n_nodes();
    for (int i = 0; i < num; i++)
    {
        double u = points[2*i + 0];
        double v = points[2*i + 1];
        tri_shape(u, v, &values[nno*i]);
    }
}

void cigma::Tri::grad_shape(int num, double *points, double *values)
{
    const int nno = n_nodes();
    const int celldim = n_celldim();
    const int stride = nno * celldim;
    for (int i = 0; i < num; i++)
    {
        double u = points[2*i + 0];
        double v = points[2*i + 1];
        tri_grad_shape(u, v, &values[stride*i]);
    }
}


//void cigma::Tri::xyz2uvw(double xyz[3], double uvw[3]) {}


bool cigma::Tri::interior(double u, double v, double w)
{
    #define ZERO    (-1.0e-6)
    #define ONE     (1 - ZERO)

    if ((u < ZERO) || (v < ZERO) || (u > (ONE - v)))
    {
        return false;
    }
    return true;

    #undef ZERO
    #undef ONE
}

// ---------------------------------------------------------------------------
