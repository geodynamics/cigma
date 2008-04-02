#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cassert>

#include "CompareCmd.h"
#include "StringUtils.h"

#include "FE_Field.h"
#include "PointField.h"
#include "ZeroField.h"
#include "ExtField.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

CompareCmd::CompareCmd()
{
    name = "compare";

    // fields
    field_a = 0;
    field_b = 0;

    // integration mesh
    meshPart = 0;
    quadrature = 0;

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

    opt->setOption("second",'b');

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
            in = (char *)"residuals-debug.vtk";
        }
        else
        {
            cerr << "compare: Please specify an output file "
                 << "for the residuals (missing --output)" << endl;
            exit(1);
        }
    }
    outputFile = in;


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
    quadratureReader.load_args(opt, "quadrature");
    if (opt->getFlag("debug"))
    {
        // 
        // Assign defaults if we're in debug mode.
        // This will override any command line settings from load_args()
        //
        if (firstReader.fieldPath == "")
            firstReader.fieldPath = "./tests/strikeslip_tet4_1000m_t0.vtk:displacements_t0";
        if (secondReader.fieldPath == "")
            secondReader.fieldPath = "./tests/strikeslip_hex8_1000m_t0.vtk:displacements_t0";
    }

    /* Validate these arguments and complain about missing ones */
    firstReader.validate_args("compare");
    secondReader.validate_args("compare");
    quadratureReader.validate_args("compare");


    /* Initialize list of known fields */
    fieldSet.initialize();

    /* Load the first field */
    firstReader.verbose = true;
    firstReader.setFieldSet(&fieldSet);
    firstReader.load_field();
    field_a = firstReader.field;
    assert(field_a != 0);
    if (field_a->getType() == Field::NULL_FIELD)
    {
        cerr << "compare: Could not load the first field!" << endl;
        exit(1);
    }

    /* Load the second field */
    secondReader.verbose = true;
    secondReader.setFieldSet(&fieldSet);
    secondReader.load_field();
    field_b = secondReader.field;
    assert(field_b != 0);
    if (field_b->getType() == Field::NULL_FIELD)
    {
        cerr << "compare: Could not load the second field!" << endl;
        exit(1);
    }

    /* Override constraints when comparing against zero */
    if (firstReader.fieldPathIsZero() || secondReader.fieldPathIsZero())
    {
        ZeroField *zero;
        if (firstReader.fieldPathIsZero() && !secondReader.fieldPathIsZero())
        {
            zero = static_cast<ZeroField*>(field_a);
            zero->set_shape(field_b->n_dim(), field_b->n_rank());
        }
        else if (!firstReader.fieldPathIsZero() && secondReader.fieldPathIsZero())
        {
            zero = static_cast<ZeroField*>(field_b);
            zero->set_shape(field_a->n_dim(), field_a->n_rank());
        }
        else
        {
            cerr << "compare: Ambiguous domain & range dimensions "
                 << "(both fields are zero)" << endl;
            exit(1);
        }
    }

    /* Check constraints on the two fields. */
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

    /* Reuse integration mesh? */
    if (!quadratureReader.meshPartReader.has_paths())
    {
        if (field_a->getType() == Field::FE_FIELD)
        {
            FE_Field *fa = static_cast<FE_Field*>(field_a);
            meshPart = fa->getMesh();
        }
    }

    /* Load quadrature rule */
    quadratureReader.verbose = verbose;
    quadratureReader.set_mesh(meshPart);
    quadratureReader.load_quadrature();
    quadrature = quadratureReader.quadrature;
    if (quadrature == 0)
    {
        cerr << "compare: Could not load quadrature points" << endl;
        exit(1);
    }


    /* Update reference to mesh */
    meshPart = quadratureReader.meshPart;
    if (meshPart == 0)
    {
        cerr << "compare: Could not load integration mesh!" << endl;
        exit(1);
    }


    /* Update field_a now that we've loaded the quadrature */
    if (field_a->getType() == Field::FE_FIELD)
    {
        FE_Field *fa = static_cast<FE_Field*>(field_a);
        fa->set_fe(quadrature);
    }


    /* Allocates residuals array to fit meshPart */
    residuals->set_mesh(meshPart);


    /* done with configure() step */
    return;
}


// ---------------------------------------------------------------------------

void CompareCmd::start_timer(int nel, const char *firstcol)
{
    if (verbose)
    {
        cout << setprecision(5);
        timer.print_header(cout, firstcol);
        timer.start(nel);
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

void compare(CompareCmd *env, FE_Field *field_a, FE_Field *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing FE_Field against FE_Field" << endl;
    }
    assert(env->quadrature != 0);

    QuadratureRule *qr = env->quadrature;
    Residuals *residuals = env->residuals;

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

    field_a->fe->initialize_basis_tab();
    residuals->reset_accumulator();

    // main loop
    env->start_timer(nel,"elts");
    for (int e = 0; e < nel; e++)
    {
        qr->select_cell(e);
        field_a->tabulate_element(e, phi_a.data);
        bool cb = field_b->Field::eval(*(qr->points), phi_b);
        double err2 = qr->L2(phi_a, phi_b);
        double vol = qr->meshPart->cell->volume();
        residuals->update(e, sqrt(err2)/vol);
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
        cout << "Comparing FE_Field against PointField" << endl;
    }
    assert(env->quadrature != 0);

    QuadratureRule *qr = env->quadrature;
    Residuals *residuals = env->residuals;

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
    env->start_timer(nel, "elts");

    // main loop
    for (int e = 0; e < nel; e++)
    {
        qr->select_cell(e);
        for (int q = 0; q < nq; q++)
        {
            double *globalPoint = (*(qr->points))[q];
            bool ca = field_a->eval(globalPoint, &values_a[q*valdim]);
            bool cb = field_b->eval(globalPoint, &values_b[q*valdim]);
        }
        double err2 = qr->L2(phi_a, phi_b);
        double vol = qr->meshPart->cell->volume();
        residuals->update(e, sqrt(err2)/vol);
        env->update_timer(e);
    }
    env->end_timer();

    // clean up
    delete [] values_a;
    delete [] values_b;
}

void compare(CompareCmd *env, Field *field_a, Field *field_b)
{
    const bool debug = false;

    if (env->verbose)
    {
        cout << "Comparing Field against Field" << endl;
    }
    assert(env->quadrature != 0);

    QuadratureRule *qr = env->quadrature;
    Residuals *residuals = env->residuals;

    // dimensions
    int nel = qr->meshPart->nel;
    int nq = qr->points->n_points();
    int ndim = field_a->n_dim();
    int valdim = field_a->n_rank();

    // field values at quadrature points
    Points phi_a, phi_b;
    double *values_a = new double[nq * valdim];
    double *values_b = new double[nq * valdim];
    phi_a.set_data(values_a, nq, valdim);
    phi_b.set_data(values_b, nq, valdim);

    residuals->reset_accumulator();
    env->start_timer(nel, "elts");

    // main loop
    for (int e = 0; e < nel; e++)
    {
        qr->select_cell(e);
        for (int q = 0; q < nq; q++)
        {
            double *globalPoint = (*(qr->points))[q];
            field_a->eval(globalPoint, &values_a[q*valdim]);
            field_b->eval(globalPoint, &values_b[q*valdim]);
            if (debug)
            {
                int j;
                cerr << e << " " << q << "  ";
                for (j = 0; j < ndim; j++)
                {
                    cerr << globalPoint[j] << " ";
                }
                cerr << "\t";
                for (j = 0; j < valdim; j++)
                {
                    cerr << values_a[valdim*q + j] << " ";
                }
                cerr << "\t";
                for (j = 0; j < valdim; j++)
                {
                    cerr << values_b[valdim*q + j] << " ";
                }
                cerr << endl;
            }
        }
        double err2 = qr->L2(phi_a, phi_b);
        double vol = qr->meshPart->cell->volume();
        residuals->update(e, sqrt(err2)/vol);
        env->update_timer(e);
    }
    env->end_timer();

    // clean up
    delete [] values_a;
    delete [] values_b;
}


// ---------------------------------------------------------------------------

int CompareCmd::run()
{
    // some basic checks before we begin
    assert(field_a != 0);
    assert(field_b != 0);
    assert(meshPart != 0);
    assert(quadrature != 0);

    if (verbose)
    {
        cout << endl;
    }

    // double type dispatching
    const bool double_dispatch = false;
    if (double_dispatch)
    {
        Field::FieldType ta = field_a->getType();
        Field::FieldType tb = field_b->getType();
        
        if ((ta == Field::FE_FIELD) && (tb == Field::FE_FIELD))
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
        else
        {
            compare(this, field_a, field_b);
        }
    }
    else
    {
        compare(this, field_a, field_b);
    }

    // report global error
    double L2 = residuals->L2();
    double max_residual = residuals->max();
    cout << setprecision(12) << endl;
    cout << "Global Norms of Residuals" << endl;
    cout << "  L2 = " << L2 << endl;
    cout << "  MaxResidual = " << max_residual << endl;
    cout << endl;

    // write out data
    cout << "Creating file " << outputFile << endl;
    residuals->write(outputFile.c_str());

    return 0;
}
// ---------------------------------------------------------------------------
