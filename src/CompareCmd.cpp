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
using namespace cigma;



// ---------------------------------------------------------------------------

cigma::CompareCmd::CompareCmd()
{
    name = "compare";

    // integrating mesh
    mesh = 0;
    quadrature = 0;
    qr = 0;

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
    /* XXX: for now, don't delete
    if (qr != 0)
    {
        delete qr; // XXX
    } // */

    if (residuals != 0) delete residuals;
}


// ---------------------------------------------------------------------------

void cigma::CompareCmd::setupOptions(AnyOption *opt)
{
    //std::cout << "Calling cigma::CompareCmd::setupOptions()" << std::endl;

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
    //std::cout << "Calling cigma::CompareCmd::configure()" << std::endl;

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
    //field_a->fe = new FE();
    //field_a->fe->set_cell(field_a->meshPart->cell);
    cout << "first field path = " << firstIO.field_path << endl;
    cout << "first field dimensions = "
         << field_a->meshPart->nel << " cells, "
         << field_a->meshPart->nno << " nodes, "
         << field_a->meshPart->cell->n_nodes() << " dofs/cell, "
         << "rank " << field_a->n_rank() << endl;

    secondIO.load();
    field_b = secondIO.field;
    assert(field_b != 0);
    //field_b->fe = new FE();
    //field_b->fe->set_cell(field_b->meshPart->cell);
    cout << "second field path = " << secondIO.field_path << endl;
    cout << "second field dimensions = "
         << field_b->meshPart->nel << " cells, "
         << field_b->meshPart->nno << " nodes, "
         << field_b->meshPart->cell->n_nodes() << " dofs/cell, "
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
    //field_a->fe->set_quadrature(quadrature); // XXX: eliminate need for this statement


    field_a->fe = new FE();
    field_a->fe->set_mesh(field_a->meshPart);
    //field_a->fe->set_quadrature_points(field_a->meshPart->cell->default_quadrature_points);
    field_a->fe->set_quadrature_points(quadrature); // XXX


    field_b->fe = new FE();
    field_b->fe->set_mesh(field_b->meshPart);
    //field_b->fe->set_quadrature_points(field_b->meshPart->cell->default_quadrature_points);


    // XXX: if field_a has a quadrature rule, reuse it
    // XXX: however, this complicates our destructor...
    qr = field_a->fe;
    assert(field_a->meshPart == mesh); // XXX


    /* XXX: when neither field_a or field_b have a mesh, need own QuadratureRule instance
    assert(mesh != 0);
    assert(field_a->meshPart == 0);
    assert(field_b->meshPart == 0);
    qr = new QuadratureRule();
    qr->set_mesh(mesh);
    qr->set_quadrature_points(quadrature);
    // */



    return;
}


// ---------------------------------------------------------------------------

void compare(CompareCmd *env, MeshPart *mesh, FE_Field *field_a, FE_Field *field_b)
{
    assert(env != 0);
    const int output_frequency = env->output_frequency;
    const bool verbose = env->verbose;

    // XXX: remove need for this assert stmt ...
    assert(mesh == field_a->meshPart);

    // XXX: remove need for env...consider moving this function back to run() 
    ResidualField *residuals = env->residuals;

    // XXX: we need need to initialize qr much earlier, so we avoid
    // assuming that we can load the quadrature points from field_a->fe object
    //QuadraturePoints *quadrature = field_a->fe->quadrature;
    //QuadratureRule *qr = new QuadratureRule();
    //qr->meshPart = mesh;
    //qr->set_quadrature_points(quadrature);
    QuadratureRule *qr = env->qr;   // XXX: replace MeshPart fn arg by QuadratureRule
    assert(qr != 0);
    assert(qr->points != 0);

    // dimensions
    int nel = qr->meshPart->nel;
    int nq = qr->points->n_points();
    int valdim = field_a->n_rank();

    // field values at quadrature points
    Points phi_a, phi_b;
    double *values_a = new double[nq * valdim];
    double *values_b = new double[nq * valdim];
    phi_a.set_data(values_a, nq, valdim);
    phi_b.set_data(values_b, nq, valdim);

    // reset error accumulator
    residuals->zero_out();

    // start timer
    Timer timer;
    if (verbose)
    {
        cout << std::setprecision(5);
        timer.print_header(cout, "elts");
        timer.start(nel);
        timer.update(0);
        cout << timer;
    }

    for (int e = 0; e < nel; e++)
    {
        // update cell data
        qr->select_cell(e);

        // ... calculate phi_a
        //field_a->Field::eval(*(qr->points), phi_a);
        field_a->tabulate_element(e, phi_a.data);   // XXX: call when field_b is an instance of FE_Field

        // ... calculate phi_b
        field_b->Field::eval(*(qr->points), phi_b);
        //field_b->tabulate_element(e, phi_b.data); // XXX: call when field_b->meshPart, field_a->meshPart,
                                                    //      and qr->meshPart are all identical...
                                                    //      this will result in redundant call to get_cell_dofs().
                                                    //      is it worth it to optimize this? it would avoid
                                                    //      jumping around the connectivity array a second time..

        // apply quadrature rule
        double err = qr->L2(phi_a, phi_b);

        // update error accumulator
        residuals->update(e, err);

        // debug info
        if (verbose && ((e+1) % output_frequency == 0))
        {
            timer.update(e+1);
            cout << timer;
        }
    }

    if (verbose)
    {
        timer.update(nel);
        cout << timer << endl;
    }

    // report global error
    double L2 = residuals->L2();
    cout << setprecision(12);
    cout << "L2 = " << L2 << endl;

    // write out data
    residuals->write_vtk(env->output_path.c_str());

    // clean up
    delete [] values_a;
    delete [] values_b;
}

/* XXX: first, time the effects of introducing branches in the main loop
void compare(MeshPart *mesh, FE_Field *field_a, Field *field_b,
             ResidualField *residuals, int output_frequency)
{
}

void compare(FE_Field *field_a, Field *field_b,
             ResidualField *residuals, int output_frequency)
{
}

void compare(FE_Field *field_a, PointField *field_b,
             ResidualField *residuals, int output_frequency)
{
}

void compare(MeshPart *mesh, Field *field_a, PointField *field_b,
             ResidualField *residuals, int output_frequency)
{
}
// */


//* XXX: new run() method
int cigma::CompareCmd::run()
{
    //std::cout << "Calling cigma::CompareCmd::run()" << std::endl;

    // 
    // XXX: need to fail gracefully at this point, instead of throwing
    // a nasty assert statement whenever something unexpected happens.
    // clean up memory management so that we can use exceptions, and
    // verify that all resources are properly finalized.
    //


    // start with the obvious checks
    assert(quadrature != 0);
    assert(mesh != 0);
    assert(qr != 0);
    assert(field_a != 0);
    assert(field_b != 0);


    // make sure the field dimensions match
    assert(field_a->n_dim() == field_a->n_dim());
    assert(field_a->n_rank() == field_b->n_rank());


    // XXX: call the appropriate compare method based on the field types
    // XXX: enclose statements in a try-catch-finally clause
    compare(this, mesh, field_a, field_b);


    return 0;
} // */


// ---------------------------------------------------------------------------
