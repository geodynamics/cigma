#include <iostream>
#include <cassert>
#include <string>
#include <cmath>
#include "Residuals.h"
#include "VtkWriter.h"
#include "HdfWriter.h"
#include "HdfAttr.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

Residuals::Residuals()
{
    nel = 0;
    epsilon = 0;
    meshPart = 0;

    global_error = 0.0;
    max_residual = 0.0;
}

Residuals::~Residuals()
{
    if (epsilon != 0) delete [] epsilon;
}

// ---------------------------------------------------------------------------

void Residuals::set_mesh(MeshPart *meshPart)
{
    assert(meshPart != 0);
    this->meshPart = meshPart;
    this->nel = meshPart->nel;
    epsilon = new double[nel];
}

// ---------------------------------------------------------------------------

void Residuals::reset_accumulator()
{
    assert(nel > 0);
    assert(epsilon != 0);

    global_error = 0.0;
    max_residual = 0.0;

    for (int e = 0; e < nel; e++)
    {
        epsilon[e] = 0.0;
    }
}

void Residuals::update(int e, double cell_residual)
{
    assert(cell_residual >= 0);

    // remember residual on this cell
    epsilon[e] = cell_residual;

    // keep track of max residual
    if (cell_residual > max_residual)
    {
        max_residual = cell_residual;
    }

    // XXX: generalize to global_error = norm_sum(global_error, cell_residual)
    global_error += cell_residual * cell_residual;
}

double Residuals::L2()
{
    return sqrt(global_error); // XXX: generalize to norm_pow(global_error)
}

double Residuals::max()
{
    return max_residual;
}

// ---------------------------------------------------------------------------

void Residuals::write(const char *filename)
{
    assert(meshPart != 0); // XXX: residuals w/o mesh?
    assert(meshPart->nel == nel);

    string output_filename = filename;

    VtkWriter writer;
    writer.open(output_filename.c_str());
    writer.write_points(meshPart->coords, meshPart->nno, meshPart->nsd);
    writer.write_cells(meshPart->connect, meshPart->nel, meshPart->ndofs);
    writer.write_cell_types(meshPart->nsd, meshPart->nel, meshPart->ndofs); // XXX: call from w/i write_cells()
    writer.write_cell_data("epsilon", epsilon, nel, 1);
    writer.close();

    const bool debug = false;
    if (debug)
    {
        int ierr;
        herr_t status;

        HdfWriter hdfWriter;
        ierr = hdfWriter.open("residuals-debug.h5");
        ierr = hdfWriter.write_dataset("residuals", epsilon, nel, 1);
        //status = HdfAttr::set_double(hdfWriter.h5.file_id, "L2", this->L2());
        //status = HdfAttr::set_double(hdfWriter.h5.file_id, "Linf", this->max());
        ierr = hdfWriter.close();
    }
}

// ---------------------------------------------------------------------------
