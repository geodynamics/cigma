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
    void set_quadrature_points(QuadraturePoints *pts);

public:
    void select_cell(int e);
    void update_jxw();

public:
    double apply(Points &f);
    double L2(Points &f, Points &g);

public:
    MeshPart *meshPart;
    QuadraturePoints *points;
    double *jxw;
};


#endif
