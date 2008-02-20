#include <iostream>
#include <cassert>
#include "CompareCmd.h"
#include "StringUtils.h"

#include "MeshPart.h"
#include "Tet.h"
#include "Hex.h"
#include "Numeric.h"
#include "AnnLocator.h"

#include "HdfReader.h"
#include "VtkReader.h"
#include "VtkWriter.h"

#include "Timer.h"
#include "Misc.h"


using namespace std;



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
    opt->addUsage("     -r  --rule       Quadrature rule location");
    opt->addUsage("     -o  --output     Output file");

    /* setup flags and options */
    opt->setFlag("help", 'h');
    opt->setFlag("debug"); // XXX: uses specific defaults

    // options for mesh
    opt->setOption("mesh");
    opt->setOption("mesh-coordinates");
    opt->setOption("mesh-connectivity");

    // options for quadrature
    opt->setOption("rule",'r');
    opt->setOption("rule-order");
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
    opt->setOption("output",'o');

    // other options
    opt->setFlag("verbose");
    opt->setOption("output-frequency",'f');
}


void cigma::CompareCmd::configure(AnyOption *opt)
{
    std::cout << "Calling cigma::CompareCmd::configure()" << std::endl;

    string field_prefix;
    std::string inputstr;
    char *in;

    /*
     * Initialization order:
     *  Load First field
     *      If FE_Field
     *          Load DofsA
     *          Load MeshA if req'd
     *          Load RuleA if req'd
     *      Else
     *          Load Analytic Field
     *  Load Second field
     *      If FE_Field
     *          Load DofsB
     *          Load MeshB if req'd
     *          Load RuleB if req'd
     *      Else
     *          Load Analytic Field
     *  Load Integration mesh
     *  Load Quadrature rule
     */


    /* Gather up the expected command line arguments */

    load_args(opt, &meshIO, "mesh");
    load_args(opt, &quadratureIO, "rule");
    load_args(opt, &firstIO, "first");
    load_args(opt, &secondIO, "second");
    load_args(opt, &residualsIO, "output");


    /* Validate these arguments and complain about missing ones */

    if (opt->getFlag("debug"))
    {
        // 
        // assign defaults if we're in debug mode. this overrides
        // the command line settings from load_args()
        //
        if (firstIO.field_path == "")
            firstIO.field_path = "./tests/strikeslip_tet4_1000m_t0.vtk:displacements_t0";

        if (secondIO.field_path == "")
            secondIO.field_path = "./tests/strikeslip_hex8_1000m_t0.vtk:displacements_t0";

        if (residualsIO.field_path == "")
            residualsIO.field_path = "foo.vtk";
    }

    validate_args(&residualsIO, "compare");
    validate_args(&firstIO, "compare");
    validate_args(&secondIO, "compare");
    validate_args(&meshIO, "compare");
    validate_args(&quadratureIO, "compare");


    /* Load the datasets into memory */

    firstIO.load();
    field_a = firstIO.field;
    assert(field_a != 0);
    field_a->fe = new FE();
    field_a->fe->set_cell(field_a->meshPart->cell);
    cout << "first field path = " << firstIO.field_path << endl;
    cout << "first field dimensions = "
         << field_a->meshPart->nel << " cells, "
         << field_a->meshPart->nno << " nodes, "
         << field_a->fe->cell->n_nodes() << " dofs/cell, "
         << "rank " << field_a->n_rank() << endl;

    secondIO.load();
    field_b = secondIO.field;
    assert(field_b != 0);
    field_b->fe = new FE();
    field_b->fe->set_cell(field_b->meshPart->cell);
    cout << "second field path = " << secondIO.field_path << endl;
    cout << "second field dimensions = "
         << field_b->meshPart->nel << " cells, "
         << field_b->meshPart->nno << " nodes, "
         << field_b->fe->cell->n_nodes() << " dofs/cell, "
         << "rank " << field_b->n_rank() << endl;


    /* XXX: if no --mesh option was specified, get mesh from the
     * first field. if the first field has no mesh, (e.g. specified
     * by an analytic soln), then try using the second field's mesh.
     * if we still don't have a mesh that we can use for the integration,
     * then exit with an error suggesting the user to specify the --mesh
     * option.
     */
    meshIO.load();
    mesh = meshIO.meshPart;
    if (mesh == 0)
    {
        mesh = firstIO.field->meshPart;
    }
    assert(mesh != 0);


    quadratureIO.load(mesh->cell);
    quadrature = quadratureIO.quadrature;
    if (quadrature == 0)
    {
        quadrature = field_a->fe->quadrature;
    }
    assert(quadrature != 0);

    // XXX: perhaps quadrature data should be loaded first!
    // move this line back into FieldIO load() method
    field_a->fe->set_quadrature(quadrature);


    /* Determine the output-frequency option */

    verbose = opt->getFlag("verbose");
    if (verbose)
    {
        output_frequency = 1000;
    }

    in = opt->getValue("output-frequency");
    if (in != 0)
    {
        inputstr = in;
        string_to_int(inputstr, output_frequency);
    }

    if (output_frequency == 0)
    {
        // XXX: emit warning, or quit?
        if (opt->getValue("output-frequency") != 0)
        {
            cerr << "compare: Warning: ignoring option --output-frequency" << endl;
        }
        verbose = false;
    }

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
    //assert(writer != 0);


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

    Timer timer;
    if (verbose)
    {
        std::cout << std::setprecision(4);
        timer.print_header(std::cout, "elts");
        timer.start(nel);
        timer.update(0);
        std::cout << timer;
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
        // XXX: use tabulation instead of calling eval repeatedly!
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

        // XXX: debug info
        if (verbose && ((e+1) % output_frequency == 0))
        {
            timer.update(e+1);
            std::cout << timer;
        }
    }

    if (verbose)
    {
        timer.update(nel);
        std::cout << timer << std::endl;
    }

    L2 = sqrt(L2);
    std::cout << std::setprecision(12);
    std::cout << "L2 = " << L2 << std::endl;


    /* write out data */
    /* use residualsIO
    residuals = new FE_Field();
    residuals->fe = 0;
    residuals->meshPart = mesh;
    residuals->dofHandler = new DofHandler();
    residuals->dofHandler->nno = nel;   //XXX: In DofHandler, rename nno to num
    residuals->dofHandler->ndim = 1;
    residuals->dofHandler->dofs = epsilon;

    residualsIO.field = residuals;
    residualsIO.save();

    //writer->open(output_filename);
    //writer->write_field(residuals);
    //writer->close();

    // */

    //* write out data
    {
        int nno = mesh->nno;
        int nsd = mesh->nsd;
        int ndofs = mesh->ndofs;
        double *coords = mesh->coords;
        int *connect = mesh->connect;

        string output_filename = residualsIO.field_path;
        string output_name = "epsilon";

        // XXX: create a cell-based ResidualField class

        std::cout << "Creating file " << output_filename << std::endl;
        //VtkUgSimpleWriter *writer = new VtkUgSimpleWriter();
        VtkWriter *writer = new VtkWriter();
        writer->open(output_filename);
        writer->write_header();
        writer->write_points(coords, nno, nsd);
        writer->write_cells(connect, nel, ndofs);
        writer->write_cell_types(nsd, nel, ndofs);
        writer->write_cell_data(output_name.c_str(), epsilon, nel, 1);
        writer->close();
        //delete writer;
    } // */


    /* clean up */
    //delete residuals->dofHandler;
    //delete residuals;
    //residualsIO.field = 0;

    delete [] epsilon;


    return 0;
}
