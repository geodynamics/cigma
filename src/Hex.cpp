#include "Hex.h"
#include <cassert>

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

cigma::Hex::Hex()
{
    const int nno = 8;
    const int nsd = 3;
    const int celldim = 3;
    double verts[nno * celldim] = {
        -1.0, -1.0, -1.0,
        +1.0, -1.0, -1.0,
        +1.0, +1.0, -1.0,
        -1.0, +1.0, -1.0,
        -1.0, -1.0, +1.0,
        +1.0, -1.0, +1.0,
        +1.0, +1.0, +1.0,
        -1.0, +1.0, +1.0
    };

    set(nsd, celldim, nno);
    set_reference_vertices(verts, nno);
}

cigma::Hex::~Hex()
{
}


/*
 * @param points points is an [num x nsd] array (in)
 * @param values values is an [num x ndofs] array (out)
 *
 */
void cigma::Hex::shape(int num, double *points, double *values)
{
    assert(ndofs > 0);
    for (int i = 0; i < num; i++)
    {
        double u = points[3*i + 0];
        double v = points[3*i + 1];
        double w = points[3*i + 2];
        hex_shape(u, v, w, &values[ndofs*i]);
    }
}

/*
 *
 * @param points points is an [num x celldim] array (in)
 * @param values values is an [num x ndofs x celldim] array (out)
 */
void cigma::Hex::grad_shape(int num, double *points, double *values)
{
    assert(ndofs > 0);
    for (int i = 0; i < num; i++)
    {
        double u = points[3*i + 0];
        double v = points[3*i + 1];
        double w = points[3*i + 2];
        hex_grad_shape(u, v, w, &values[ndofs*celldim*i]);
    }
}

/*
 *
 */
bool cigma::Hex::interior(double u, double v, double w)
{
    #define ZERO    (-1.0e-6)
    #define ONE     (1-ZERO)

    if ((u < -ONE) || (v < -ONE) || (w < -ONE) ||
        (u > +ONE) || (v > +ONE) || (w > +ONE))
    {
        return false;
    }
    return true;

    #undef ONE
    #undef ZERO
}
