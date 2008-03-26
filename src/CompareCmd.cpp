#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cassert>
#include "CompareCmd.h"
#include "StringUtils.h"
#include "FE_Field.h"
#include "PointField.h"
#include "ExtField.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

CompareCmd::CompareCmd()
{
    name = "compare";

    // integrating mesh
    meshPart = 0;
    quadrature = 0;
    qr = 0;

    // fields
    field_a = 0;
    field_b = 0;

    // residuals
    residuals = new Residuals();

    // parameters
    verbose = false;
    outputFrequency = 0;
}

CompareCmd::~CompareCmd()
{
    if (residuals != 0)
    {
        delete residuals;
        residuals = 0;
    }
}

// ---------------------------------------------------------------------------

void CompareCmd::setupOptions(AnyOption *opt)
{
    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("   cigma compare [options]");
    opt->addUsage("     -a  --first         First field");
    opt->addUsage("     -b  --second        Second field");
    opt->addUsage("     -q  --quadrature    Quadrature rule");
    opt->addUsage("     -o  --output        Output file for residuals");

    /* setup flags and options */
    opt->setFlag("help",'h');
    opt->setFlag("debug"); // XXX: uses specific defaults


    /* 
     * options for quadrature
     */
    opt->setOption("quadrature",'q');
    opt->setOption("quadrature-order");
    opt->setOption("quadrature-points");
    opt->setOption("quadrature-weights");
    opt->setOption("quadrature-mesh");
    opt->setOption("quadrature-mesh-coordinates");
    opt->setOption("quadrature-mesh-connectivity");


    /* 
     * options for first field
     */

    opt->setOption("first",'a');

    // FE_Field options
    opt->setOption("first-mesh");
    opt->setOption("first-mesh-coordinates");
    opt->setOption("first-mesh-connectivity");

    // PointField options
    opt->setOption("first-points");             
    opt->setOption("first-values");

    // ExtField options
    opt->setOption("first-function");


    /* 
     * options for second field (same form as first)
     */

    opt->setOption("second",'a');

    // FE_Field options
    opt->setOption("second-mesh");
    opt->setOption("second-mesh-coordinates");
    opt->setOption("second-mesh-connectivity");

    // PointField options
    opt->setOption("second-points");             
    opt->setOption("second-values");

    // ExtField options
    opt->setOption("second-function");


    /*
     * options for output
     */

    opt->setOption("output",'o');

    // other options
    opt->setFlag("verbose",'v');
    opt->setOption("output-frequency",'f');

}

void CompareCmd::configure(AnyOption *opt)
{
    string fieldPrefix;
    string inputstr;
    char *in;

    /* Check if --verbose was enabled */
    
    verbose = opt->getFlag("verbose");
    if (verbose)
    {
        outputFrequency = 1000;
    }


    /* Determine the --output-frequency option */
    
    in = opt->getValue("output-frequency");
    if (in != 0)
    {
        inputstr = in;
        string_to_int(inputstr, outputFrequency);
    }
    if (outputFrequency == 0)
    {
        // emit warning, or quit?
        if (opt->getValue("output-frequency") != 0)
        {
            cerr << "compare: Need a positive integer for the option --output-frequency" << endl;
            exit(1);
        }
        verbose = false;
    }


    /* Determine the --output option */

    in = opt->getValue("output");
    if (in == 0)
    {
        if (opt->getFlag("debug"))
        {
            // provide default name when in debug mode
            in = (char *)"foo.vtk";
        }
        else
        {
            cerr << "compare: Please specify an output file "
                 << "for the residuals (missing --output)" << endl;
            exit(1);
        }
    }
    outputPath = in;


    /*
     * Initialization order:
     *  Load first field
     *  Load second field
     *  Load quadrature mesh
     *  Load quadrature rule
     */

    /* Gather up expected command line arguments */

    firstReader.load_args(opt, "first");
    secondReader.load_args(opt, "second");
    meshPartReader.load_args(opt, "quadrature-mesh");
    quadratureReader.load_args(opt, "quadrature");

    /* Validate these arguments and complain about missing ones */

    if (opt->getFlag("debug"))
    {
        // assign defaults if we're in debug mode. this overrides
        // the command line settings from load_args()
        if (firstReader.fieldPath == "")
            firstReader.fieldPath = "./tests/strikeslip_tet4_1000m_t0.vtk:displacements_t0";
        if (secondReader.fieldPath == "")
            secondReader.fieldPath = "./tests/strikeslip_hex8_1000m_t0.vtk:displacements_t0";
    }

    firstReader.validate_args("compare");
    secondReader.validate_args("compare");
    meshPartReader.validate_args("compare");
    quadratureReader.validate_args("compare");


    /* Load the fields */

    firstReader.verbose = true;
    firstReader.load_field();
    field_a = firstReader.field;
    if (field_a == 0)
    {
        cerr << "compare: Could not load the first field!" << endl;
        exit(1);
    }

    secondReader.verbose = true;
    secondReader.load_field();
    field_b = secondReader.field;
    if (field_b == 0)
    {
        cerr << "compare: Could not load the second field!" << endl;
        exit(1);
    }

    if (field_a->n_dim() != field_b->n_dim())
    {
        cerr << "compare: Domain dimensions do not match: "
             << "first->dim = " << field_a->n_dim() << ", "
             << "second->dim = " << field_b->n_dim() << endl;
        exit(1);
    }

    if (field_a->n_rank() != field_b->n_rank())
    {
        cerr << "compare: Range dimensions do not match: "
             << "first->rank = " << field_a->n_rank() << ", "
             << "second->rank = " << field_b->n_dim() << endl;
        exit(1);
    }


    /* Load the integration mesh -- XXX: move this operation to QuadratureReader */
    meshPartReader.load_mesh();
    meshPart = meshPartReader.meshPart;
    if (meshPart == 0)
    {
        if (field_a->getType() == Field::FE_FIELD)
        {
            FE_Field *a = static_cast<FE_Field*>(field_a);
            meshPart = a->meshPart;
        }
    }
    if (meshPart == 0)
    {
        cerr << "compare: Could not load the quadrature mesh!" << endl;
        exit(1);
    }
    
    /* Load the quadrature rule */
    //quadratureReader.verbose = true;
    quadratureReader.load_quadrature(meshPart->cell);
    quadrature = quadratureReader.quadrature;
    if (quadrature == 0)
    {
        cerr << "compare: Could not load quadrature points" << endl;
        exit(1);
    }

    //qr = ...; // XXX:

    if (field_a->getType() == Field::FE_FIELD)
    {
        FE_Field *a = static_cast<FE_Field*>(field_a);
        a->fe = new FE();
        a->fe->set_mesh(a->meshPart);
        a->fe->set_quadrature_points(quadrature);

        qr = a->fe; // XXX
    }

    if (field_b->getType() == Field::FE_FIELD)
    {
        FE_Field *b = static_cast<FE_Field*>(field_b);
        b->fe = new FE();
        b->fe->set_mesh(b->meshPart);
        //b->fe->set_quadrature_points(quadrature);
    }

    /* this allocates residuals->epsilon[] array */
    residuals->set_mesh(meshPart);
}


// ---------------------------------------------------------------------------

void CompareCmd::start_timer()
{
    if (verbose)
    {
        cout << setprecision(5);
        timer.print_header(cout, "elts");
        timer.start(meshPart->nel);
        timer.update(0);
        cout << timer;
    }
}

void CompareCmd::update_timer(int e)
{
    if (verbose && ((e+1) % outputFrequency == 0))
    {
        timer.update(e+1);
        cout << timer;
    }
}

void CompareCmd::end_timer()
{
    if (verbose)
    {
        timer.update(meshPart->nel);
        cout << timer << endl;
    }
}


// ---------------------------------------------------------------------------

void compare(CompareCmd *env, PointField *field_a, FE_Field *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing PointField with FE_Field" << endl;
    }
    assert(false);
    env->start_timer();
    env->end_timer();
}

void compare(CompareCmd *env, FE_Field *field_a, FE_Field *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing FE_Field with FE_Field" << endl;
    }

    Residuals *residuals = env->residuals;
    QuadratureRule *qr = env->qr;
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

    residuals->reset_accumulator();
    env->start_timer();
    for (int e = 0; e < nel; e++)
    {
        qr->select_cell(e);
        field_a->tabulate_element(e, phi_a.data);
        field_b->Field::eval(*(qr->points), phi_b);
        double err = qr->L2(phi_a, phi_b);
        residuals->update(e, err);
        env->update_timer(e);
    }
    env->end_timer();

    // clean up
    delete [] values_a;
    delete [] values_b;

}

void compare(CompareCmd *env, FE_Field *field_a, PointField *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing FE_Field with PointField" << endl;
    }
    assert(false);
    env->start_timer();
    env->end_timer();
}

void compare(CompareCmd *env, FE_Field *field_a, ExtField *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing FE_Field with ExtField" << endl;
    }
    assert(false);
    env->start_timer();
    env->end_timer();
}

void compare(CompareCmd *env, FE_Field *field_a, Field *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing FE_Field with Field" << endl;
    }
    assert(false);
    env->start_timer();
    env->end_timer();
}

void compare(CompareCmd *env, Field *field_a, Field *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing Field with Field" << endl;
    }
    assert(false);
    env->start_timer();
    env->end_timer();
}

int CompareCmd::run()
{
    // some basic checks before we begin
    assert(meshPart != 0);
    assert(quadrature != 0);
    assert(qr != 0);
    assert(field_a != 0);
    assert(field_b != 0);

    int status;


    Field::FieldType ta = field_a->getType();
    Field::FieldType tb = field_b->getType();
    
    if ((ta == Field::POINT_FIELD) && (tb == Field::FE_FIELD))
    {
        PointField *fa = static_cast<PointField*>(field_a);
        FE_Field *fb = static_cast<FE_Field*>(field_b);
        compare(this, fa, fb);
    }
    else if ((ta == Field::FE_FIELD) && (tb == Field::FE_FIELD))
    {
        FE_Field *a = static_cast<FE_Field*>(field_a);
        FE_Field *b = static_cast<FE_Field*>(field_b);
        compare(this, a, b);
    }
    else if ((ta == Field::FE_FIELD) && (tb == Field::POINT_FIELD))
    {
        FE_Field *a = static_cast<FE_Field*>(field_a);
        PointField *b = static_cast<PointField*>(field_b);
        compare(this, a, b);
    }
    else if ((ta == Field::FE_FIELD) && (tb == Field::EXT_FIELD))
    {
        FE_Field *a = static_cast<FE_Field*>(field_a);
        ExtField *b = static_cast<ExtField*>(field_b);
        compare(this, a, b);
    }
    else if ((ta == Field::FE_FIELD) && (tb == Field::USER_FIELD))
    {
        FE_Field *a = static_cast<FE_Field*>(field_a);
        Field *b = field_b;
        compare(this, a, b);
    }
    else
    {
        compare(this, field_a, field_b);
    }


    // report global error
    double L2 = residuals->L2();
    cout << setprecision(12);
    cout << "L2 = " << L2 << endl;

    // write out data
    residuals->write_vtk(outputPath.c_str());

    return 0;
}
// ---------------------------------------------------------------------------
