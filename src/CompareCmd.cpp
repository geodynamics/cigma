#include <iostream>
#include <cassert>
#include <ctime>
#include "CompareCmd.h"
#include "StringUtils.h"

//#include "VtkUgMeshPart.h"
#include "MeshPart.h"
#include "Tet.h"
#include "Hex.h"
#include "Numeric.h"
#include "AnnLocator.h"

//#include "VtkUgSimpleWriter.h"
#include "HdfReader.h"
#include "VtkReader.h"

#include "Misc.h"



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

    // readers & writers
    readerA = 0;
    readerB = 0;
    readerM = 0;
    readerQ = 0;
    writer = 0;

    // parameters
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

    // options for mesh
    opt->setOption("mesh");
    opt->setOption("mesh-coordinates");
    opt->setOption("mesh-connectivity");

    // options for quadrature
    opt->setOption("order");
    opt->setOption("rule");
    opt->setOption("rule-points");
    opt->setOption("rule-weights");

    // options for first field, in the form /path/to/file:/path/to/dset
    opt->setOption("first",'a');
    opt->setOption("first-mesh");
    opt->setOption("first-mesh-coordinates");
    opt->setOption("first-mesh-connectivity");

    // option for second field, in the form /path/to/file:/path/to/dset
    opt->setOption("second",'b');
    opt->setOption("second-mesh");
    opt->setOption("second-mesh-coordinates");
    opt->setOption("second-mesh-connectivity");

    // options for output
    opt->setOption("output");
    opt->setOption("output-frequency",'f');

    // other options
    opt->setFlag("verbose");
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
    load_reader(&readerA, extA);
    readerA->open(inputfileA);


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
    load_reader(&readerB, extB);
    readerB->open(inputfileB);

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
    string output_ext;
    string output_fileroot;
    output_filename = in;
    path_splitext(output_filename, output_fileroot, output_ext);
    load_writer(&writer, output_ext);
    output_name = "epsilon";


    verbose = opt->getFlag("verbose");

    in = opt->getValue("output-frequency");
    if (in == 0)
    {
        in = "1000";
    }
    inputstr = in;
    string_to_int(inputstr, output_frequency);


    /*
     * Initialization order:
     *  Load Integration mesh
     *  Load Quadrature rule
     *  Load First field
     *      If FE_Field
     *          Load MeshA
     *          Load DofsB
     *      Else
     *          Load Analytic Field
     *  Load Second field
     *      If FE_Field
     *          Load MeshB
     *          Load DofsB
     *      Else
     *          Load Analytic Field
     */



    field_a = new FE_Field();
    load_field(inputfileA, locationA, readerA, field_a);
    std::cout << "first field location = " << locationA << std::endl;
    std::cout << "first field inputfile = " << inputfileA << std::endl;
    std::cout << "first field extension = " << extA << std::endl;
    std::cout << "first field dimensions = "
              << field_a->meshPart->nel << " cells, "
              << field_a->meshPart->nno << " nodes, " 
              << field_a->fe->cell->n_nodes() << " dofs/cell, rank "
              << field_a->n_rank() << std::endl;

    field_b = new FE_Field();
    load_field(inputfileB, locationB, readerB, field_b);
    std::cout << "second field location = " << locationB << std::endl;
    std::cout << "second field inputfile = " << inputfileB << std::endl;
    std::cout << "second field extension = " << extB << std::endl;
    std::cout << "second field dimensions = "
              << field_b->meshPart->nel << " cells, "
              << field_b->meshPart->nno << " nodes, " 
              << field_b->fe->cell->n_nodes() << " dofs/cell, rank "
              << field_b->n_rank() << std::endl;

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
        mesh->select_cell(e);
        //mesh->get_cell_coords(e, cell->globverts);
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
