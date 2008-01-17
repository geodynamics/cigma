#ifndef __CELL_H__
#define __CELL_H__

#include "Numeric.h"

namespace cigma
{
    class Cell;
}


/*
 * Cell class
 */
class cigma::Cell
{

public:
    Cell();
    virtual ~Cell();

public:
    virtual int n_nodes() = 0;
    virtual int n_celldim() = 0;
    virtual int n_dim() = 0;

public:
    void set_reference_vertices(double *vertices, int num_vertices);
    void set_global_vertices(double *vertices, int num_vertices, int nsd);

public:
    virtual void shape(int num, double *points, double *values) = 0;
    virtual void grad_shape(int num, double *points, double *values) = 0;

    double jacobian(double *point, double jac[3][3]);
    double jacobian(double u, double v, double w, double jac[3][3]);

    void interpolate(double *dofs, double *point, double *value, int valdim);
    void interpolate_grad(double *dofs, double *point, double *value, int stride=1, double invjac[3][3]=0);

    virtual void xyz2uvw(double xyz[3], double uvw[3]);
    void uvw2xyz(double uvw[3], double xyz[3]);

    //virtual double volume() = 0;

public:
    void bbox(double *min, double *max);
    void centroid(double c[3]);
    virtual bool interior(double u, double v, double w) = 0;
    virtual bool interior2(double x, double y, double z);


public:
    double *refverts;   // [nno x celldim]
    double *globverts;  // [nno x nsd]

};


// ---------------------------------------------------------------------------
#endif
