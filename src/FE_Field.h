#ifndef __FE_FIELD_H__
#define __FE_FIELD_H__

#include "Field.h"
#include "DofHandler.h"
#include "MeshPart.h"
#include "FE.h"

namespace cigma
{
    class FE_Field;
}


/**
 * @brief Finite Element Field object
 *
 */

class cigma::FE_Field : public Field
{
public:
    FE_Field();
    ~FE_Field();

public:
    int n_dim() { return dim; }
    int n_rank() { return rank; }

public:
    void eval(double *point, double *value);

public:
    void set_quadrature_rule(QuadratureRule *rule);
    void tabulate_element(int e, double *values);

public:
    void get_cell_dofs(int cellIndex, double *cellDofs);

public:
    
    int dim;
    int rank;
    DofHandler *dofHandler;
    MeshPart *meshPart;
    FE *fe;
};

#endif
