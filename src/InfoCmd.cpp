#include <string>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include "InfoCmd.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

InfoCmd::InfoCmd()
{
    name = "info";
}

InfoCmd::~InfoCmd()
{
}


// ---------------------------------------------------------------------------

void InfoCmd::setupOptions(AnyOption *opt)
{
    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("   cigma info [options]");
    opt->addUsage("     --mesh      Path to mesh");
    opt->addUsage("     --field     Path to field data");
    opt->addUsage("     --output    Output file name");
    
    /* setup flags and options */
    opt->setFlag("help",'h');
    
    opt->setOption("mesh");
    opt->setOption("mesh-coordinates");
    opt->setOption("mesh-connectivity");
    opt->setOption("output",'o');
}

void InfoCmd::configure(AnyOption *opt)
{
    char *in;
    string inputstr;

    /* Determine the --output option */
    in = opt->getValue("output");
    if (in == 0)
    {
        cerr << "info: Please specify an output file" << endl;
        exit(1);
    }
    outputFilename = in;

    /* Load mesh */
    if (opt->getValue("mesh") != 0)
    {
        meshPartReader.load_args(opt, "mesh");
        meshPartReader.validate_args("info");
        meshPartReader.load_mesh();
        meshPart = meshPartReader.meshPart;
        if (meshPart == 0)
        {
            cerr << "info: Could not load mesh." << endl;
            exit(1);
        }
        meshPart->set_cell();
    }

    /* Load field */
    if (opt->getValue("field") != 0)
    {
        fieldReader.load_args(opt, "field");
        fieldReader.validate_args("info");
        fieldReader.load_field();
        field = fieldReader.field;
        if (field == 0)
        {
            cerr << "info: Could not load field." << endl;
            exit(1);
        }
    }
}

// ---------------------------------------------------------------------------


int InfoCmd::run()
{
    int e;
    int nel = meshPart->nel;
    double *vols = 0; // XXX: use auto_ptr?
    int status;

    cout << "Creating file " << outputFilename << endl;
    status = hdfWriter.open(outputFilename.c_str());
    if (status < 0)
    {
        cerr << "info: Could not create " << outputFilename << endl;
        exit(1);
    }


    vols = new double[nel];
    for (e = 0; e < nel; e++)
    {
        meshPart->select_cell(e);
        vols[e] = meshPart->cell->volume();
    }
    status = hdfWriter.write_dataset("/volume", vols, nel, 1);
    if (status < 0)
    {
        cerr << "info: Could not write /volume dataset" << endl;
        exit(1);
    }

    status = hdfWriter.close();
    delete [] vols;

    return 0;
}

// ---------------------------------------------------------------------------
