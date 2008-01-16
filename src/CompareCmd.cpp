#include <iostream>
#include <cassert>
#include "CompareCmd.h"
#include "StringUtils.h"
#include "VtkUgSimpleWriter.h"
#include "VtkUgMeshPart.h"
#include "Tet.h"
#include "Hex.h"



// ---------------------------------------------------------------------------

cigma::CompareCmd::CompareCmd()
{
    name = "compare";

    // integrating mesh
    mesh = 0;
    quadrature = 0;

    // fields
    field_a = 0;
    field_b = 0;
    residuals = 0; //XXX: create ResidualField class?
}

cigma::CompareCmd::~CompareCmd()
{
}


// ---------------------------------------------------------------------------

void cigma::CompareCmd::setupOptions(AnyOption *opt)
{
    std::cout << "Calling cigma::CompareCmd::setupOptions()" << std::endl;

    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("   cigma compare [options]");
    opt->addUsage("     -a  --fieldA     First field location");
    opt->addUsage("     -b  --fieldB     Second field location");
    opt->addUsage("         --order      Quadrature order");
    opt->addUsage("         --output     Output file");

    /* setup flags and options */
    opt->setFlag("help", 'h');

    // options for first field, in the form /path/to/file:/path/to/dset
    opt->setOption("first",'a');

    // option for second field, in the form /path/to/file:/path/to/dset
    opt->setOption("second",'b');

    // options for quadrature
    opt->setOption("order");

    // options for output
    opt->setOption("output");
}

static void load_field(std::string inputfile,
                       std::string location,
                       cigma::VtkUgReader &reader,
                       cigma::FE_Field *field)
{
    const int tet_nno = 8;
    const int tet_nsd = 3;
    double tet_qpts[8*3] = {
        -0.68663473, -0.72789005, -0.75497035,
        -0.83720867, -0.85864055,  0.08830369,
        -0.86832263,  0.13186633, -0.75497035,
        -0.93159441, -0.4120024 ,  0.08830369,
         0.16949513, -0.72789005, -0.75497035,
        -0.39245447, -0.85864055,  0.08830369,
        -0.50857335,  0.13186633, -0.75497035,
        -0.74470688, -0.4120024 ,  0.08830369 };
    double tet_qwts[8] = {
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

    int nno, nsd;
    int nel, ndofs;
    double *coords;
    int *connect;

    int dofs_nno, dofs_valdim;
    double *dofs;

    reader.open(inputfile);
    reader.get_coordinates(&coords, &nno, &nsd);
    reader.get_connectivity(&connect, &nel, &ndofs);
    //reader.get_dofs(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);
    //reader.get_vector_point_data(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);
    reader.get_scalar_point_data(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);
    //reader.get_point_data(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);

    field->dim = nsd;
    field->rank = dofs_valdim;
    field->meshPart = new cigma::VtkUgMeshPart();
    field->meshPart->set_coordinates(coords, nno, nsd);
    field->meshPart->set_connectivity(connect, nel, ndofs);

    switch (ndofs)
    {
    case 4:
        field->fe = new cigma::FE();
        field->fe->cell = new cigma::Tet();
        field->fe->quadrature = new cigma::Quadrature();
        field->fe->quadrature->set_quadrature(tet_qpts, tet_qwts, tet_nno, tet_nsd);
        field->fe->quadrature->set_globaldim(3);
        field->fe->jxw = new double[tet_nno];
        field->fe->basis_tab = new double[tet_nno * ndofs];
        field->fe->basis_jet = new double[tet_nno * ndofs * 3];
        field->meshPart->cell = field->fe->cell;
        break;
    case 8:
        field->fe = new cigma::FE();
        field->fe->cell = new cigma::Hex();
        field->fe->quadrature = new cigma::Quadrature();
        field->fe->quadrature->set_quadrature(hex_qpts, hex_qwts, hex_nno, hex_nsd);
        field->fe->quadrature->set_globaldim(3);
        field->fe->jxw = new double[hex_nno];
        field->fe->basis_tab = new double[hex_nno * ndofs];
        field->fe->basis_jet = new double[hex_nno * ndofs * 3];
        field->meshPart->cell = field->fe->cell;
        break;
    }
    assert(field->fe != 0);
    assert(field->fe->cell != 0);

    field->dofHandler = new cigma::DofHandler();
    field->dofHandler->meshPart = field->meshPart;
    field->dofHandler->set_data(dofs, dofs_nno, dofs_valdim);

    return;
}

void cigma::CompareCmd::configure(AnyOption *opt)
{
    std::cout << "Calling cigma::CompareCmd::configure()" << std::endl;

    std::string inputA, inputB;
    std::string inputfileA, inputfileB;
    std::string extA, extB;
    char *in;

    bool debug = true;

    in = opt->getValue("fieldA");
    if (in == 0)
    {
        in = "./tests/strikeslip_tet4_1000m_t0.vtk:displacements_t0";
        if (!debug)
        {
            std::cerr << "compare: Please specify the option --fieldA" << std::endl;
            exit(1);
        }
    }
    inputA = in;
    parse_dataset_path(inputA, locationA, inputfileA, extA);
    std::cout << "fieldA location = " << locationA << std::endl;
    std::cout << "fieldA inputfile = " << inputfileA << std::endl;
    std::cout << "fieldA extension = " << extA << std::endl;

    in = opt->getValue("fieldB");
    if (in == 0)
    {
        in = "./tests/strikeslip_hex8_1000m_t0.vtk:displacements_t0";
        if (!debug)
        {
            std::cerr << "compare: Please specify the option --fieldB" << std::endl;
            exit(1);
        }
    }
    inputB = in;
    parse_dataset_path(inputB, locationB, inputfileB, extB);
    std::cout << "fieldB location = " << locationB << std::endl;
    std::cout << "fieldB inputfile = " << inputfileB << std::endl;
    std::cout << "fieldB extension = " << extB << std::endl;

    in = opt->getValue("output");
    if (in == 0)
    {
        in = "foo.vtk";
        if (!debug)
        {
            std::cerr << "compare: Please specify the option --output" << std::endl;
            exit(1);
        }
    }
    output_filename = in;
    output_name = "epsilon";


    field_a = new FE_Field();
    load_field(inputfileA, locationA, readerA, field_a);

    field_b = new FE_Field();
    load_field(inputfileB, locationB, readerB, field_b);


    /* if no mesh specified, get it from fieldA
     * if fieldA has no mesh (e.g. specified by analytic soln), then
     * swap fieldA and fieldB, and try again.
     * if fieldA still has no mesh, then produce error if no mesh
     * was specified to begin with.
     */
    mesh = field_a->meshPart;
    quadrature = field_a->fe->quadrature;

    return;
}

int cigma::CompareCmd::run()
{
    std::cout << "Calling cigma::CompareCmd::run()" << std::endl;
    assert(mesh != 0);
    assert(quadrature != 0);
    assert(field_a != 0);
    assert(field_b != 0);
    assert(field_a->n_dim() == field_a->n_dim());
    assert(field_a->n_rank() == field_b->n_rank());


    Cell *cell_a = field_a->fe->cell;
    Cell *cell_b = field_b->fe->cell;
    assert(cell_a->n_celldim() == cell_b->n_celldim());

    // indices
    int e,q;

    // dimensions
    int nel = mesh->nel;
    int nq = quadrature->n_points();
    //int celldim = cell_a->n_celldim();
    int valdim = field_a->n_rank();

    // local data;
    //double *jxw = new double[nq];
    double *phi_a = new double[nq * valdim];
    double *phi_b = new double[nq * valdim];
    double *jxw = field_a->fe->jxw;
    
    // norm
    double L2 = 0.0;
    double *epsilon = new double[nel];

    //FE *fe;
    Cell *cell = cell_a;

    const int eltPeriod = 1000;
    const int eltMax = 1000;
    for (e = 0; e < nel; e++)
    {
        /* XXX: debug
        if (e % eltPeriod == 0)
        {
            std::cout << e << " " << std::flush;
            if (e == eltMax) { break; }
        } // */

        // update cell data
        mesh->get_cell_coords(e, cell->globverts);
        //cell->set_global_vertices(...);
        
        // obtain global points from current quadrature rule
        quadrature->apply_refmap(cell);

        field_a->tabulate();

        // ... calculate phi_a[]
        for (q = 0; q < nq; q++)
        {
            field_a->eval((*quadrature)[q], &phi_a[valdim*q]);
        }

        // ... calculate phi_b[]
        for (q = 0; q < nq; q++)
        {
            field_b->eval((*quadrature)[q], &phi_b[valdim*q]);
        }

        // ... apply quadrature rule
        double err = 0.0;
        for (q = 0; q < nq; q++)
        {
            for (int i = 0; i < valdim; i++)
            {
                int qi = valdim*q + i;
                err += jxw[q] * SQR(phi_a[qi] - phi_b[qi]);
            }
        }

        epsilon[e] = err;
        L2 += err;
    }
    L2 = sqrt(L2);

    /* write out data */
    {
        int nno = mesh->nno;
        int nsd = mesh->nsd;
        int ndofs = mesh->ndofs;
        double *coords = mesh->coords;
        int *connect = mesh->connect;

        /* XXX: create a cell-based ResidualField class */
        std::cout << "Creating file " << output_filename << std::endl;
        VtkUgSimpleWriter *writer = new VtkUgSimpleWriter();
        writer->open(output_filename);
        writer->write_header();
        writer->write_points(coords, nno, nsd);
        writer->write_cells(connect, nel, ndofs);
        writer->write_cell_types(nsd, nel, ndofs);
        writer->write_cell_data(output_name.c_str(), epsilon, nel, 1);
        writer->close();
        //delete writer;
    }

    /* clean up */
    delete [] epsilon;

    return 0;
}
