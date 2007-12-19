#ifndef __QUADRATURE_RULE_H__
#define __QUADRATURE_RULE_H__

namespace cigma
{
    class QuadratureRule;
}

/**
 * @brief Base class for quadrature schemes
 *
 */

class cigma::QuadratureRule
{
public:
    QuadratureRule();
    ~QuadratureRule();

public:
    void set_quadrature(double *points, double *weights, int npts, int nsd);
    void get_quadrature(double **points, double **weights, int *npts, int *nsd);

public:
    int nq;
    int dim;
    double *qpts;
    double *qwts;
};

#endif
