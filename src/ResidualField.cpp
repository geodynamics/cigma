#include <iostream>
#include <string>
#include "ResidualField.h"
#include "VtkWriter.h"

using namespace std;

// ---------------------------------------------------------------------------

cigma::ResidualField::ResidualField()
{
    nel = 0;
    epsilon = 0;
    meshPart = 0;
}

cigma::ResidualField::~ResidualField()
{
    if (epsilon != 0) delete [] epsilon;
}

// ---------------------------------------------------------------------------

void cigma::ResidualField::set_mesh(MeshPart *meshPart)
{
    assert(meshPart != 0);
    this->meshPart = meshPart;
    this->nel = meshPart->nel;
    epsilon = new double[nel];
}

// ---------------------------------------------------------------------------

void cigma::ResidualField::write_vtk(const char *filename)
{
    assert(meshPart != 0); // XXX: add support for dumping residuals w/o mesh
    assert(meshPart->nel == nel);

    string output_filename = filename;
    cout << "Creating file " << output_filename << endl;

    VtkWriter writer;
    writer.open(output_filename); // XXX: change signature to 'const char *'
    writer.write_header();
    writer.write_points(meshPart->coords, meshPart->nno, meshPart->nsd);
    writer.write_cells(meshPart->connect, meshPart->nel, meshPart->ndofs);
    writer.write_cell_types(meshPart->nsd, meshPart->nel, meshPart->ndofs); // XXX: call from w/i write_cells()
    writer.write_cell_data("epsilon", epsilon, nel, 1);
    writer.close();
}

// ---------------------------------------------------------------------------
