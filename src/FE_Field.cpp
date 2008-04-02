#include <cassert>
#include "FE_Field.h"

using namespace cigma;

// ---------------------------------------------------------------------------

FE_Field::FE_Field()
{
    dim = 0;
    rank = 0;
    fe = 0;
    meshPart = 0;
    dofHandler = 0;
}

FE_Field::~FE_Field()
{
}

// ---------------------------------------------------------------------------

void FE_Field::set_fe(QuadratureRule *rule)
{
    assert(rule != 0);
    this->fe = rule;
}

void FE_Field::set_mesh(MeshPart *mesh)
{
    assert(mesh != 0);
    this->meshPart = mesh;
}

// ---------------------------------------------------------------------------

void FE_Field::get_cell_dofs(int cellIndex, double *cellDofs)
{
    assert(dofHandler != 0);
    assert(meshPart != 0);

    assert(0 <= cellIndex);
    assert(cellIndex < (meshPart->nel));

    const int ndofs = meshPart->cell->n_nodes();
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

bool FE_Field::eval(double *point, double *value)
{
    assert(meshPart != 0);
    assert(meshPart->cell != 0);
    Cell *cell = meshPart->cell;

    // find the cell which contains given point
    int e;
    bool found_cell = false;
    found_cell = meshPart->find_cell(point, &e);
    if (!found_cell)
        return false;

    // use dofs as weights on the shape function values
    const int ndofs = cell->n_nodes();
    int valdim = n_rank();
    double field_dofs[ndofs * valdim]; // XXX

    get_cell_dofs(e, field_dofs);

    double uvw[3];
    cell->xyz2uvw(point,uvw);
    cell->interpolate(field_dofs, uvw, value, valdim);

    return true;
}

void FE_Field::tabulate_element(int e, double *values)
{
    assert(fe != 0);
    assert(fe->points != 0);
    assert(fe->basis_tab != 0);
    assert(meshPart != 0);

    Cell *cell = meshPart->cell;
    assert(cell != 0);

    QuadraturePoints *points = fe->points;
    assert(points != 0);
    int nq = points->n_points();


    // tabulate the function values
    const int valdim = n_rank();
    const int ndofs = cell->n_nodes();
    double dofs[ndofs * valdim]; // XXX


    get_cell_dofs(e, dofs); // XXX: do we split this function so that this call
                            //      is independent from the following loop?

    for (int q = 0; q < nq; q++)
    {
        double *N = &(fe->basis_tab[ndofs*q]);
        for (int i = 0; i < valdim; i++)
        {
            double sum = 0.0;
            for (int j = 0; j < ndofs; j++)
            {
                sum += dofs[i + valdim*j] * N[j];
            }
            values[valdim*q + i] = sum;
        }
    }
}

// ---------------------------------------------------------------------------
