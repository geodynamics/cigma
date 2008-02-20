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

    // residuals
    residuals = new ResidualField();

    // parameters
    verbose = false;
    output_frequency = 0;
}

cigma::CompareCmd::~CompareCmd()
{
    delete residuals;
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


    /* Check if --verbose was enabled */

    verbose = opt->getFlag("verbose");
    if (verbose)
    {
        output_frequency = 1000;
    }

    /* Determine the --output-frequency option */

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

    /* Determine the --output option */

    in = opt->getValue("output");
    if (in == 0)
    {
        if (opt->getFlag("debug"))
        {
            // XXX: provide default name when in debug mode
            in = (char *)"foo.vtk";
        }
        else
        {
            cerr << "compare: Please specify the option --output" << endl;
            exit(1);
        }
    }
    output_path = in;


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
    }

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
    residuals->set_mesh(mesh);


    /* Now load the quadrature rule. If no rule is specified on the
     * command line, a default rule is assigned based on the type of
     * the cell. Also, an exception is thrown if the specified a rule
     * does not conform geometrically to the interior of the cell.
     */
    quadratureIO.load(mesh->cell);
    quadrature = quadratureIO.quadrature;
    assert(quadrature != 0);
    field_a->fe->set_quadrature(quadrature);


    return;
}


int cigma::CompareCmd::run()
{
    std::cout << "Calling cigma::CompareCmd::run()" << std::endl;
    assert(mesh != 0);
    assert(quadrature != 0);
    assert(field_a != 0);
    assert(field_b != 0);

    // make sure the field dimensions match
    // XXX: need graceful failure mode
    assert(field_a->n_dim() == field_a->n_dim());
    assert(field_a->n_rank() == field_b->n_rank());

    Cell *cell_a = field_a->fe->cell;
    Cell *cell_b = field_b->fe->cell;
    assert(cell_a->n_celldim() == cell_b->n_celldim());


    // indices
    int e,q;
    int i,j;

    // dimensions
    int nel = mesh->nel;
    int nq = quadrature->n_points();
    int ndofs = cell_a->n_nodes();
    int valdim = field_a->n_rank();

    // local data;
    double *jxw = field_a->fe->jxw;
    double *phi_a = new double[nq * valdim]; // XXX: not needed when using tabulation
    double *phi_b = new double[nq * valdim];
    
    // norm
    double L2 = 0.0;
    double *epsilon = residuals->epsilon;


    // XXX: don't forget about the case when (mesh != field_a->meshPart)
    // Regardless, this cell pointer needs to come from the integrating
    // mesh object, not from field_a.
    Cell *cell = cell_a;


    // start timer
    Timer timer;
    if (verbose)
    {
        std::cout << std::setprecision(5);
        timer.print_header(std::cout, "elts");
        timer.start(nel);
        timer.update(0);
        std::cout << timer;
    }

    // XXX: time to move this main loop into its own function
    // so we can use polymorphic dispatch on the argument types
    for (e = 0; e < nel; e++)
    {
        // update cell data
        mesh->select_cell(e);
        
        // obtain global points from current quadrature rule
        quadrature->apply_refmap(cell);


        // ... calculate phi_a[]
        // XXX: using eval() -- applicable in general (Field obj)
        //for (q = 0; q < nq; q++)
        //{
        //    field_a->eval((*quadrature)[q], &phi_a[valdim*q]);
        //}

        // ... calculate phi_a[]
        // XXX: using tabulation -- applicable to FE_Field obj
        field_a->tabulate_element(e, phi_a);

        // ... calculate phi_b[]
        for (q = 0; q < nq; q++)
        {
            field_b->eval((*quadrature)[q], &phi_b[valdim*q]);
        }

        // evaluate jacobian at known quadrature points
        field_a->fe->update_jxw();  // XXX: somehow, this method needs to be attached
                                    // to the MeshPart object, so maybe it needs its
                                    // own FE object after all?

        // ... apply quadrature rule
        double err = 0.0;
        for (q = 0; q < nq; q++)
        {
            for (i = 0; i < valdim; i++)
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
    residuals->write_vtk(output_path.c_str());


    /* clean up */
    delete [] phi_a;
    delete [] phi_b;


    return 0;
}


// ---------------------------------------------------------------------------
