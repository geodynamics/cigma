#include <cassert>
#include "FE_Field.h"

// ---------------------------------------------------------------------------

cigma::FE_Field::FE_Field()
{
    dim = 0;
    rank = 0;
    fe = 0;
    meshPart = 0;
    dofHandler = 0;
}

cigma::FE_Field::~FE_Field()
{
}

// ---------------------------------------------------------------------------

void cigma::FE_Field::get_cell_dofs(int cellIndex, double *cellDofs)
{
    assert(fe != 0);
    assert(dofHandler != 0);
    assert(meshPart != 0);
    assert(0 <= cellIndex);
    assert(cellIndex < (meshPart->nel));

    const int ndofs = fe->cell->n_nodes();
    const int valdim = n_rank();

    int *n = &(meshPart->connect[ndofs * cellIndex]);
    for (int i = 0; i < ndofs; i++)
    {
        for (int j = 0; j < valdim; j++)
        {
            cellDofs[valdim * i + j] = dofHandler->dofs[valdim * n[i] + j];
        }
    }
}

// ---------------------------------------------------------------------------

void cigma::FE_Field::eval(double *point, double *value)
{
    assert(fe != 0);
    assert(meshPart != 0);

    // get reference cell object from fe
    Cell *cell = fe->cell;
    assert(cell != 0);

    // find the cell which contains given point
    int e;
    bool found_cell = false;
    found_cell = meshPart->find_cell(point, &e);
    assert(found_cell);

    // use dofs as weights on the shape function values
    const int ndofs = cell->n_nodes();
    int valdim = n_rank();

    //double globalCellCoords[cell_nno * cell_nsd];
    //meshPart->get_cell_coords(e, globalCellCoords);
    //cell->interpolate(globalCellCoords, point, value, valdim);

    double field_dofs[ndofs * valdim];
    get_cell_dofs(e, field_dofs);
    double uvw[3];
    cell->xyz2uvw(point,uvw);
    cell->interpolate(field_dofs, uvw, value, valdim);
}

void cigma::FE_Field::tabulate_element(int e, double *values)
{
    assert(fe != 0);
    assert(meshPart != 0);

    Cell *cell = fe->cell;
    assert(cell != 0);

    QuadraturePoints *quadrature = fe->quadrature;
    assert(quadrature != 0);
    int nq = quadrature->n_points();
    double *qpts = quadrature->qpts;

    // tabulate the function values
    int i,j;
    const int valdim = n_rank();
    const int ndofs = cell->n_nodes();
    double dofs[ndofs * valdim]; // XXX

    get_cell_dofs(e, dofs);

    for (int q = 0; q < nq; q++)
    {
        double *N = &(fe->basis_tab[ndofs*q]);
        for (i = 0; i < valdim; i++)
        {
            double sum = 0.0;
            for (j = 0; j < ndofs; j++)
            {
                sum += dofs[i + valdim*j] * N[j];
            }
            values[valdim*q + i] = sum;
        }
    }
}

// ---------------------------------------------------------------------------
