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

Tri::~Tri()
{
}

void Tri::qr_default(double **wts, double **pts, int *npts, int *ndim)
{
    // tri_qr(5)
    const int tri_nno = 9;
    const int tri_celldim = 2;
    double tri_qpts[tri_nno * tri_celldim] = {
        -0.79456469, -0.82282408,
        -0.86689186, -0.18106627,
        -0.95213774,  0.57531892,
        -0.08858796, -0.82282408,
        -0.40946686, -0.18106627,
        -0.78765946,  0.57531892,
         0.61738877, -0.82282408,
         0.04795814, -0.18106627,
        -0.62318119,  0.57531892
    };
    double tri_qwts[tri_nno] = {
        0.22325768,  0.25471234,  0.07758553,  0.35721229,  0.40753974,
        0.12413685,  0.22325768,  0.25471234,  0.07758553
    };

    int i,j,n;

    for (i = 0; i < tri_nno; i++)
    {
        for (j = 0; j < tri_celldim; j++)
        {
            n = tri_celldim * i + j;
            tri_qpts[n] += 1;
            tri_qpts[n] *= 0.5;
        }
    }

    *npts = tri_nno;
    *ndim = tri_celldim;
    *wts = new double[(*npts)];
    *pts = new double[(*npts) * (*ndim)];
    for (i = 0; i < tri_nno; i++)
    {
        (*wts)[i] = tri_qwts[i];
        for (j = 0; j < (*ndim); j++)
        {
            n = (*ndim) * i + j;
            (*pts)[n] = tri_qpts[n];
        }
    }
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
