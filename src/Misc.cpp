#include <iostream>
#include <iomanip>
#include <cassert>
#include <string>

#include "Misc.h"
#include "AnnLocator.h"


using namespace std;
using namespace cigma;


// ---------------------------------------------------------------------------

double pick_from_interval(double a, double b)
{
    return a + (b - a) * rand()/(RAND_MAX + 1.0);
}


void bbox_random_point(double minpt[3], double maxpt[3], double x[3])
{
    const int nsd = 3;
    for (int i = 0; i < nsd; i++)
    {
        assert(minpt[i] <= maxpt[i]);
        x[i] = pick_from_interval(minpt[i], maxpt[i]);
    }
}


// ---------------------------------------------------------------------------

void load_quadrature(Cell *cell, Quadrature *quadrature)
{
    assert(cell != 0);

    // XXX: change *_nsd to *_celldim since we are
    // talking about quadrature points in the appropriate
    // reference domain

    const int tri_nno = 1;
    const int tri_nsd = 3;
    double tri_qpts[tri_nno * tri_nsd] = {
        0.0, 0.0, 0.0
    };
    double tri_qwts[tri_nno] = {
        0.0
    };

    const int quad_nno = 1;
    const int quad_nsd = 3;
    double quad_qpts[quad_nno * quad_nsd] = {
        0.0, 0.0, 0.0
    };
    double quad_qwts[quad_nno] = {
        0.0
    };

    const int tet_nno = 8;
    const int tet_nsd = 3;
    double tet_qpts[tet_nno * tet_nsd] = {
        -0.68663473, -0.72789005, -0.75497035,
        -0.83720867, -0.85864055,  0.08830369,
        -0.86832263,  0.13186633, -0.75497035,
        -0.93159441, -0.4120024 ,  0.08830369,
         0.16949513, -0.72789005, -0.75497035,
        -0.39245447, -0.85864055,  0.08830369,
        -0.50857335,  0.13186633, -0.75497035,
        -0.74470688, -0.4120024 ,  0.08830369 };
    double tet_qwts[tet_nno] = {
        0.29583885,  0.12821632,  0.16925605,  0.07335544,  0.29583885,
        0.12821632,  0.16925605,  0.07335544 };
    for (int i = 0; i < tet_nno; i++)
    {
        for (int j = 0; j < tet_nsd; j++)
        {
            int n = tet_nsd * i + j;
            tet_qpts[n] += 1;
            tet_qpts[n] *= 0.5;
        }
    }

    const int hex_nno = 8;
    const int hex_nsd = 3;
    double hex_qpts[8*3] = {
        -0.57735027, -0.57735027, -0.57735027,
         0.57735027, -0.57735027, -0.57735027,
         0.57735027,  0.57735027, -0.57735027,
        -0.57735027,  0.57735027, -0.57735027,
        -0.57735027, -0.57735027,  0.57735027,
         0.57735027, -0.57735027,  0.57735027,
         0.57735027,  0.57735027,  0.57735027,
        -0.57735027,  0.57735027,  0.57735027 };
    double hex_qwts[8*3] = { 1.,  1.,  1.,  1.,  1.,  1.,  1.,  1. };


    switch (cell->geometry())
    {
    case Cell::TRIANGLE:
        quadrature->set_quadrature(tri_qpts, tri_qwts, tri_nno, tri_nsd);
        quadrature->set_globaldim(tri_nsd);
        break;
    case Cell::QUADRANGLE:
        quadrature->set_quadrature(quad_qpts, quad_qwts, quad_nno, quad_nsd);
        quadrature->set_globaldim(quad_nsd);
        break;
    case Cell::TETRAHEDRON:
        quadrature->set_quadrature(tet_qpts, tet_qwts, tet_nno, tet_nsd);
        quadrature->set_globaldim(tet_nsd);
        break;
    case Cell::HEXAHEDRON:
        quadrature->set_quadrature(hex_qpts, hex_qwts, hex_nno, hex_nsd);
        quadrature->set_globaldim(hex_nsd);
        break;
    }

}

void load_mesh()
{
}

void load_field()
{
}

void load_field(std::string inputfile,
                std::string location,
                cigma::VtkUgReader &reader,
                cigma::FE_Field *field)
{

    int nno, nsd;
    int nel, ndofs;
    double *coords;
    int *connect;

    int dofs_nno, dofs_valdim;
    double *dofs;

    /* XXX: For the following two cases, I need a static initializer on
     * Reader class that instantiates the right subclass based on the
     * detected filetype,
     *
     * After that, here I also need to use a switch statement on the value
     * of reader->getReaderType() in order to downcast the Reader
     * object properly...
     *
     * Having done that, I can rely on function polymorphism to call the right
     * routine: e.g choose between LoadField(VtkReader *reader, ...),
     * LoadField(HdfReader *reader, ...), LoadField(TextReader *reader)
     *
     * For detecting the filetype, one could rely only on the extension,
     * or possibly check for a magic number at the beginning of the file.
     */
    //* VtkUgReader case...
    reader.open(inputfile);
    reader.get_coordinates(&coords, &nno, &nsd);
    reader.get_connectivity(&connect, &nel, &ndofs);
    //reader.get_dofs(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);
    //reader.get_vector_point_data(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);
    reader.get_scalar_point_data(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);
    //reader.get_point_data(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);
    // */

    //* HdfReader case...
    // */


    field->dim = nsd;
    field->rank = dofs_valdim;
    //field->meshPart = new cigma::VtkUgMeshPart();
    field->meshPart = new cigma::MeshPart();
    field->meshPart->set_coordinates(coords, nno, nsd);
    field->meshPart->set_connectivity(connect, nel, ndofs);


    field->meshPart->set_cell();
    assert(field->meshPart->cell != 0);


    //* // XXX: Create locator only when necessary
    cigma::AnnLocator *locator = new cigma::AnnLocator();
    field->meshPart->set_locator(locator);
    // */


    /*
    switch (field->dim)
    {
    case 2:
        // 2D case
        switch (ndofs)
        {
        case 3:
            field->fe = new cigma::FE();
            //field->fe->cell = new cigma::Tri();
            field->fe->quadrature = new cigma::Quadrature();
            //field->fe->quadrature->set_quadrature(tri_qpts, tri_qwts, tri_nno, tri_nsd);
            //field->fe->quadrature->set_globaldim(tri_nsd);
            //field->fe->jxw = new double[tri_nno];
            //field->fe->basis_tab = new double[tri_nno * ndofs];
            //field->fe->basis_jet = new double[tri_nno * ndofs * tri_nsd];
            //field->meshPart->cell = field->fe->cell;
            break;
        case 4:
            field->fe = new cigma::FE();
            //field->fe->cell = new cigma::Quad();
            field->fe->quadrature = new cigma::Quadrature();
            //field->fe->quadrature->set_quadrature(quad_qpts, quad_qwts, quad_nno, quad_nsd);
            //field->fe->quadrature->set_globaldim(quad_nsd);
            //field->fe->jxw = new double[quad_nno];
            //field->fe->basis_tab = new double[quad_nno * ndofs];
            //field->fe->basis_jet = new double[quad_nno * ndofs * quad_nsd];
            //field->meshPart->cell = field->fe->cell;
            break;
        }
        break;

    case 3:
        // 3D case
        switch (ndofs)
        {
        case 4:
            field->fe = new cigma::FE();
            //field->fe->cell = new cigma::Tet();
            field->fe->quadrature = new cigma::Quadrature();
            //field->fe->quadrature->set_quadrature(tet_qpts, tet_qwts, tet_nno, tet_nsd);
            //field->fe->quadrature->set_globaldim(3);
            //field->fe->jxw = new double[tet_nno];
            //field->fe->basis_tab = new double[tet_nno * ndofs];
            //field->fe->basis_jet = new double[tet_nno * ndofs * tet_nsd];
            //field->meshPart->cell = field->fe->cell;
            break;
        case 8:
            field->fe = new cigma::FE();
            //field->fe->cell = new cigma::Hex();
            field->fe->quadrature = new cigma::Quadrature();
            //field->fe->quadrature->set_quadrature(hex_qpts, hex_qwts, hex_nno, hex_nsd);
            //field->fe->quadrature->set_globaldim(hex_nsd);
            //field->fe->jxw = new double[hex_nno];
            //field->fe->basis_tab = new double[hex_nno * ndofs];
            //field->fe->basis_jet = new double[hex_nno * ndofs * hex_nsd];
            //field->meshPart->cell = field->fe->cell;
            break;
        }
        break;
    }
    field->fe->cell = field->meshPart->cell;
    // */

    Quadrature *Q = new cigma::Quadrature();
    load_quadrature(field->meshPart->cell, Q);

    field->fe = new cigma::FE();
    field->fe->set_cell_quadrature(field->meshPart->cell, Q);


    assert(field->fe != 0);
    assert(field->fe->cell != 0);


    field->dofHandler = new cigma::DofHandler();
    field->dofHandler->meshPart = field->meshPart;
    field->dofHandler->set_data(dofs, dofs_nno, dofs_valdim);

    return;
}

