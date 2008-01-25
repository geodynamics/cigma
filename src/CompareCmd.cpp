#include <iostream>
#include <cassert>
#include <ctime>
#include "CompareCmd.h"
#include "StringUtils.h"
#include "VtkUgSimpleWriter.h"
//#include "VtkUgMeshPart.h"
#include "MeshPart.h"
#include "Tet.h"
#include "Hex.h"
#include "Numeric.h"
#include "AnnLocator.h"



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

    verbose = false;
    output_frequency = 0;
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
    opt->addUsage("     -a  --first      First field location");
    opt->addUsage("     -b  --second     Second field location");
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

    // other options
    opt->setFlag("verbose");
    opt->setOption("output-frequency",'f');
}

static void load_field(std::string inputfile,
                       std::string location,
                       cigma::VtkUgReader &reader,
                       cigma::FE_Field *field)
{
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


    switch (field->dim)
    {
    case 2:
        //* 2D case
        switch (ndofs)
        {
        case 3:
            field->fe = new cigma::FE();
            //field->fe->cell = new cigma::Tri();
            field->fe->quadrature = new cigma::Quadrature();
            field->fe->quadrature->set_quadrature(tri_qpts, tri_qwts, tri_nno, tri_nsd);
            field->fe->quadrature->set_globaldim(tri_nsd);
            field->fe->jxw = new double[tri_nno];
            field->fe->basis_tab = new double[tri_nno * ndofs];
            field->fe->basis_jet = new double[tri_nno * ndofs * tri_nsd];
            //field->meshPart->cell = field->fe->cell;
            break;
        case 4:
            field->fe = new cigma::FE();
            //field->fe->cell = new cigma::Quad();
            field->fe->quadrature = new cigma::Quadrature();
            field->fe->quadrature->set_quadrature(quad_qpts, quad_qwts, quad_nno, quad_nsd);
            field->fe->quadrature->set_globaldim(quad_nsd);
            field->fe->jxw = new double[quad_nno];
            field->fe->basis_tab = new double[quad_nno * ndofs];
            field->fe->basis_jet = new double[quad_nno * ndofs * quad_nsd];
            //field->meshPart->cell = field->fe->cell;
            break;
        }
        // */
        break;

    case 3:
        //* 3D case
        switch (ndofs)
        {
        case 4:
            field->fe = new cigma::FE();
            //field->fe->cell = new cigma::Tet();
            field->fe->quadrature = new cigma::Quadrature();
            field->fe->quadrature->set_quadrature(tet_qpts, tet_qwts, tet_nno, tet_nsd);
            field->fe->quadrature->set_globaldim(3);
            field->fe->jxw = new double[tet_nno];
            field->fe->basis_tab = new double[tet_nno * ndofs];
            field->fe->basis_jet = new double[tet_nno * ndofs * tet_nsd];
            //field->meshPart->cell = field->fe->cell;
            break;
        case 8:
            field->fe = new cigma::FE();
            //field->fe->cell = new cigma::Hex();
            field->fe->quadrature = new cigma::Quadrature();
            field->fe->quadrature->set_quadrature(hex_qpts, hex_qwts, hex_nno, hex_nsd);
            field->fe->quadrature->set_globaldim(hex_nsd);
            field->fe->jxw = new double[hex_nno];
            field->fe->basis_tab = new double[hex_nno * ndofs];
            field->fe->basis_jet = new double[hex_nno * ndofs * hex_nsd];
            //field->meshPart->cell = field->fe->cell;
            break;
        } // */
        break;
    }
    field->fe->cell = field->meshPart->cell;

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
    std::string inputstr;
    char *in;

    bool debug = true;

    in = opt->getValue("first");
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

    in = opt->getValue("second");
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


    verbose = opt->getFlag("verbose");

    in = opt->getValue("output-frequency");
    if (in == 0)
    {
        in = "1000";
    }
    inputstr = in;
    string_to_int(inputstr, output_frequency);


    field_a = new FE_Field();
    load_field(inputfileA, locationA, readerA, field_a);
    std::cout << "first field location = " << locationA << std::endl;
    std::cout << "first field inputfile = " << inputfileA << std::endl;
    std::cout << "first field extension = " << extA << std::endl;
    std::cout << "first field dimensions = " << field_a->meshPart->nel << " cells, " << field_a->meshPart->nno << " nodes, "  << field_a->fe->cell->n_nodes() << " dofs/cell, rank " << field_a->n_rank() << std::endl;

    field_b = new FE_Field();
    load_field(inputfileB, locationB, readerB, field_b);
    std::cout << "second field location = " << locationB << std::endl;
    std::cout << "second field inputfile = " << inputfileB << std::endl;
    std::cout << "second field extension = " << extB << std::endl;
    std::cout << "second field dimensions = " << field_b->meshPart->nel << " cells, " << field_b->meshPart->nno << " nodes, "  << field_b->fe->cell->n_nodes() << " dofs/cell, rank " << field_b->n_rank() << std::endl;

    std::cout << "outputfile = " << output_filename << std::endl;


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

    time_t t_0, t_e;
    time(&t_0);
    t_e = t_0;

    const int eltPeriod = output_frequency;
    const int eltMax = 1000;

    if (verbose)
    {
        std::cout << std::setprecision(4);
        std::cout << "elts rate mins eta total progress\n";
    }

    for (e = 0; e < nel; e++)
    {
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

        //* XXX: debug info
        if (verbose && (e % output_frequency == 0))
        {
            double elapsed_mins;
            double rate_per_min;
            double cells_per_sec;
            double remaining_mins;
            double total_mins;
            double progress;

            time(&t_e);
            elapsed_mins = (t_e - t_0) / 60.0;
            rate_per_min = elapsed_mins / (e + 1.0);
            cells_per_sec = (1.0/60.0) / rate_per_min;
            remaining_mins = (nel - e) * rate_per_min;
            total_mins = nel * rate_per_min;
            progress = 100 * elapsed_mins / total_mins;

            //std::cout << e << " " << std::flush;
            //std::cout << remaining_mins << "          " << std::flush;
            std::cout << e << " " << cells_per_sec << " " << elapsed_mins << " " << remaining_mins << " " << total_mins << " " << progress << "%";
            std::cout << "                                                                            \r" << std::flush;

            //if (e == eltMax) { break; }
        } // */

    }

    if (verbose)
    {
        std::cout << std::endl;
    }

    L2 = sqrt(L2);
    std::cout << std::setprecision(12);
    std::cout << "L2 = " << L2 << std::endl;

    /* write out data */
    {
        int nno = mesh->nno;
        int nsd = mesh->nsd;
        int ndofs = mesh->ndofs;
        double *coords = mesh->coords;
        int *connect = mesh->connect;

        /* XXX: create a cell-based ResidualField class */
        //std::cout << "Creating file " << output_filename << std::endl;
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
