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
    qpoints = 0;
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

    firstReader.load_field();
    field_a = firstReader.field;
    if (field_a == 0)
    {
        cerr << "compare: Could not load the first field!" << endl;
        exit(1);
    }

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
            FE_Field *fa = static_cast<FE_Field*>(field_a);
            meshPart = fa->meshPart;
        }
    }
    if (meshPart == 0)
    {
        cerr << "compare: Could not load the quadrature mesh!" << endl;
        exit(1);
    }
    
    /* Load the quadrature rule */

    quadratureReader.load_quadrature(meshPart->cell);
    qpoints = quadratureReader.quadrature;
    if (qpoints == 0)
    {
        cerr << "compare: Could not load quadrature points" << endl;
        exit(1);
    }

    // qr = ...; // XXX
}


// ---------------------------------------------------------------------------

void compare(CompareCmd *env, PointField *field_a, FE_Field *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing PointField with FE_Field" << endl;
    }
}

void compare(CompareCmd *env, FE_Field *field_a, FE_Field *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing FE_Field with FE_Field" << endl;
    }
}

void compare(CompareCmd *env, FE_Field *field_a, PointField *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing FE_Field with PointField" << endl;
    }
}

void compare(CompareCmd *env, FE_Field *field_a, ExtField *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing FE_Field with ExtField" << endl;
    }

}

void compare(CompareCmd *env, FE_Field *field_a, Field *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing FE_Field with Field" << endl;
    }

}

void compare(CompareCmd *env, Field *field_a, Field *field_b)
{
    if (env->verbose)
    {
        cout << "Comparing Field with Field" << endl;
    }
}

int CompareCmd::run()
{
    // start with basic checks
    assert(meshPart != 0);
    assert(qpoints != 0);
    assert(qr != 0);
    assert(field_a != 0);
    assert(field_b != 0);


    // start timer
    if (verbose)
    {
        cout << setprecision(5);
        timer.print_header(cout, "elts");
        timer.start(meshPart->nel);
        timer.update(0);
        cout << timer;
    }

    Field::FieldType a = field_a->getType();
    Field::FieldType b = field_b->getType();
    
    if ((a == Field::POINT_FIELD) && (b == Field::FE_FIELD))
    {
        PointField *fa = static_cast<PointField*>(field_a);
        FE_Field *fb = static_cast<FE_Field*>(field_b);
        compare(this, fa, fb);
    }
    else if ((a == Field::FE_FIELD) && (b == Field::FE_FIELD))
    {
        FE_Field *fa = static_cast<FE_Field*>(field_a);
        FE_Field *fb = static_cast<FE_Field*>(field_b);
        compare(this, fa, fb);

    }
    else if ((a == Field::FE_FIELD) && (b == Field::POINT_FIELD))
    {
        FE_Field *fa = static_cast<FE_Field*>(field_a);
        PointField *fb = static_cast<PointField*>(field_b);
        compare(this, fa, fb);
    }
    else if ((a == Field::FE_FIELD) && (b == Field::EXT_FIELD))
    {
        FE_Field *fa = static_cast<FE_Field*>(field_a);
        ExtField *fb = static_cast<ExtField*>(field_b);
        compare(this, fa, fb);
    }
    else if ((a == Field::FE_FIELD) && (b == Field::USER_FIELD))
    {
        FE_Field *fa = static_cast<FE_Field*>(field_a);
        Field *fb = field_b;
        compare(this, fa, fb);
    }
    else
    {
        compare(this, field_a, field_b);
    }

    if (verbose)
    {
        timer.update(meshPart->nel);
        cout << timer << endl;
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
