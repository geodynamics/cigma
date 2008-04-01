#include "Tri.h"

using namespace cigma;

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

Tri::Tri()
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

void Tri::shape(int num, double *points, double *values)
{
    const int nno = n_nodes();
    for (int i = 0; i < num; i++)
    {
        double u = points[2*i + 0];
        double v = points[2*i + 1];
        tri_shape(u, v, &values[nno*i]);
    }
}

void Tri::grad_shape(int num, double *points, double *values)
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


//void Tri::xyz2uvw(double xyz[3], double uvw[3]) {}


#define ZERO    (-1.0e-6)
#define ONE     (1 - ZERO)

bool Tri::interior(double u, double v, double w)
{

    if ((u < ZERO) || (v < ZERO) || (u > (ONE - v)))
    {
        return false;
    }
    return true;
}

#undef ZERO
#undef ONE


double Tri::volume()
{
    #define X(n) globverts[3*(n)+0]
    #define Y(n) globverts[3*(n)+1]
    return 0.5*(X(0) * Y(1) - X(0) * Y(2) - X(1) * Y(0) + X(1) * Y(2) + X(2) * Y(0) - X(2) * Y(1));
    #undef X
    #undef Y
}

// ---------------------------------------------------------------------------
