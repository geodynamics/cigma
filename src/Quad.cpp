#include "Quad.h"

using namespace cigma;

// ---------------------------------------------------------------------------

static void quad_shape(double u, double v, double s[4])
{
    s[0] = 0.25 * (1.0 - u) * (1.0 - v);
    s[1] = 0.25 * (1.0 + u) * (1.0 - v);
    s[2] = 0.25 * (1.0 + u) * (1.0 + v);
    s[3] = 0.25 * (1.0 - u) * (1.0 + v);
}

static void quad_grad_shape(double u, double v, double s[4*2])
{
    #define S(i,j) s[2*(i) + (j)]

    S(0,0) = -0.25 * (1.0 - v);
    S(0,1) = -0.25 * (1.0 - u);

    S(1,0) = +0.25 * (1.0 - v);
    S(1,1) = -0.25 * (1.0 + u);

    S(2,0) = +0.25 * (1.0 + v);
    S(2,1) = +0.25 * (1.0 + u);

    S(3,0) = -0.25 * (1.0 + v);
    S(3,1) = +0.25 * (1.0 - u);

    #undef S
}

// ---------------------------------------------------------------------------

Quad::Quad()
{
    const int quad_nno = 4;
    const int quad_celldim = 2;
    double verts[quad_nno * quad_celldim] = {
        -1.0, -1.0,
        +1.0, -1.0,
        +1.0, +1.0,
        -1.0, +1.0
    };
    set_reference_vertices(verts, quad_nno);
}

Quad::~Quad()
{
}


// ---------------------------------------------------------------------------

/*
 *
 * @param points points is an [num x celldim] array (in)
 * @param values values is an [num x ndofs] array (out)
 */
void Quad::shape(int num, double *points, double *values)
{
    const int nno = n_nodes();
    for (int i = 0; i < num; i++)
    {
        double u = points[2*i + 0];
        double v = points[2*i + 1];
        quad_shape(u, v, &values[nno*i]);
    }
}

/*
 *
 * @param points points is an [num x celldim] array (in)
 * @param values values is an [num x ndofs x celldim] array (out)
 */
void Quad::grad_shape(int num, double *points, double *values)
{
    const int nno = n_nodes();
    const int celldim = n_celldim();
    const int stride = nno * celldim;
    for (int i = 0; i < num; i++)
    {
        double u = points[2*i + 0];
        double v = points[2*i + 1];
        quad_grad_shape(u, v, &values[stride*i]);
    }
}


/*
 *
 *
 */
#define ZERO    (-1.0e-6)
#define ONE     (1-ZERO)

bool Quad::interior(double u, double v, double w)
{
    if ((u < -ONE) || (v < -ONE) ||
        (u > +ONE) || (v > +ONE))
    {
        return false;
    }
    return true;
}

#undef ONE
#undef ZERO

// ---------------------------------------------------------------------------
