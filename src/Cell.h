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
    int n_dims() const;
    int n_celldims() const;
    int n_nodes() const;


public:

    void set(int nsd, int celldim, int ndofs);

    void set_reference_vertices(const double *vertices, int num_vertices);
    void update_vertices(double *vertices, int num_vertices, int nsd);

    void set_quadrature(const double *quadpts, const double *quadwts, int num_points);
    void update_quadrature(void);

    void set_tabulation(const double *basis_tab, const double *basis_jet);
    void update_tabulation(void);

    void clear();


public:

    virtual void shape(int num, double *points, double *values) = 0;
    virtual void grad_shape(int num, double *points, double *values) = 0;

    void tabulate(void);

    double jacobian(double *point, double jac[3][3]);
    double jacobian(double u, double v, double w, double jac[3][3]);

    void interpolate(double *dofs, double *point, double *value, int valdim);
    void interpolate_grad(double *dofs, double *point, double *value, int stride=1, double invjac[3][3]=0);

    virtual void xyz2uvw(double xyz[3], double uvw[3]);
    void uvw2xyz(double uvw[3], double xyz[3]);


public:

    void bbox(double *min, double *max);
    void centroid(double c[3]);
    virtual bool interior(double u, double v, double w) = 0;


public:

    int nsd;
    int celldim;

    int nno;
    double *refverts;   // [nno x celldim]
    double *globverts;  // [nno x nsd]

    int nq;
    double *jxw;        // [nq x 1]
    double *qwts;       // [nq x 1]
    double *qpts;       // [nq x celldim]
    double *gqpts;      // [nq x nsd]

    int ndofs;
    double *basis_tab;  // [nq x ndofs]
    double *basis_jet;  // [nq x ndofs x celldim]

    bool _tabulate;
};



/*
 * Inline methods
 */
inline int cigma::Cell::n_dims() const
{
    return nsd;
}

inline int cigma::Cell::n_celldims() const
{
    return celldim;
}

inline int cigma::Cell::n_nodes() const
{
    return nno;
}


#endif
