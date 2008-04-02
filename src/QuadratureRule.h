#ifndef __QUADRATURE_RULE_H__
#define __QUADRATURE_RULE_H__


#include "MeshPart.h"
#include "QuadraturePoints.h"


namespace cigma
{
    class QuadratureRule;
}


class cigma::QuadratureRule
{
public:
    QuadratureRule();
    ~QuadratureRule();

public:
    void set_mesh(MeshPart *mesh);
    void set_quadrature(double *wts, double *pts, int npts, int ndim);
    void initialize_basis_tab();
    void initialize_basis_jet();

public:
    void select_cell(int e);
    void update_jxw();

public:
    double apply(Points &f);
    double L2(Points &f, Points &g);

public:
    MeshPart *meshPart;
    QuadraturePoints *points;
    double *jxw;        // [nq x 1]
    double *basis_tab;  // [nq x ndofs]
    double *basis_jet;  // [nq x ndofs x celldim]
};


#endif
