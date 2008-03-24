#include <iostream>
#include <cassert>
#include <string>
#include <cmath>
#include "Residuals.h"
#include "VtkWriter.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

Residuals::Residuals()
{
    nel = 0;
    epsilon = 0;
    meshPart = 0;
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

void Residuals::zero_out()
{
    assert(nel > 0);
    assert(epsilon != 0);

    global_error = 0.0;

    for (int e = 0; e < nel; e++)
    {
        epsilon[e] = 0.0;
    }
}

void Residuals::update(int e, double cell_residual)
{
    // remember residual on this cell
    epsilon[e] = cell_residual;

    // XXX: this would generalize into global_error = norm_sum(global_error, cell_residual)
    global_error += cell_residual;

}

double Residuals::L2()
{
    return sqrt(global_error); // XXX: generalize to norm_pow(global_error)
}

// ---------------------------------------------------------------------------

void Residuals::write_vtk(const char *filename)
{
    assert(meshPart != 0); // XXX: add support for dumping residuals w/o mesh
    assert(meshPart->nel == nel);

    string output_filename = filename;
    cout << "Creating file " << output_filename << endl;

    VtkWriter writer;
    writer.open(output_filename.c_str());
    //writer.write_header();
    writer.write_points(meshPart->coords, meshPart->nno, meshPart->nsd);
    writer.write_cells(meshPart->connect, meshPart->nel, meshPart->ndofs);
    writer.write_cell_types(meshPart->nsd, meshPart->nel, meshPart->ndofs); // XXX: call from w/i write_cells()
    writer.write_cell_data("epsilon", epsilon, nel, 1);
    writer.close();
}

// ---------------------------------------------------------------------------
