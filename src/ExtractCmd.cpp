#include <iostream>
#include <cassert>
#include "ExtractCmd.h"
#include "StringUtils.h"
#include "Writer.h"
#include "MeshPart.h"

using namespace std;
using namespace cigma;

// ----------------------------------------------------------------------------

ExtractCmd::ExtractCmd()
{
    name = "extract";

    coordsField = 0;
    pointsWriter = 0;
}

ExtractCmd::~ExtractCmd()
{
}


// ----------------------------------------------------------------------------

void ExtractCmd::setupOptions(AnyOption *opt)
{
    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("   cigma extract [options]");
    opt->addUsage("     --help                  Display this screen");
    opt->addUsage("     --mesh                  Input mesh file");
    opt->addUsage("     --mesh-coordinates      Path to mesh coordinates");
    opt->addUsage("     --mesh-connectivity     Path to mesh connectivity");
    opt->addUsage("     --quadrature            Quadrature rule");
    opt->addUsage("     --output                Output file");

    /* setup flags and options */

    opt->setFlag("help", 'h');
    opt->setFlag("verbose", 'v');

    opt->setOption("mesh");
    opt->setOption("mesh-coordinates");
    opt->setOption("mesh-connectivity");

    opt->setOption("quadrature", 'q');
    opt->setOption("quadrature-order");
    opt->setOption("quadrature-points");
    opt->setOption("quadrature-weights");

    opt->setOption("output", 'o');
}


void ExtractCmd::configure(AnyOption *opt)
{
    assert(opt != 0);

    string inputstr, ext;
    char *in;


    // read verbose flag
    verbose = opt->getFlag("verbose");

    /* determine the output option */
    in = opt->getValue("output");
    if (in == 0)
    {
        cerr << "extract: Please specify the option --output" << endl;
        exit(1);
    }
    inputstr = in;
    parse_dataset_path(inputstr, pointsPath, outputFilename, ext);
    pointsWriter = NewWriter(ext.c_str());
    if (pointsWriter->getType() == Writer::NULL_WRITER)
    {
        cerr << "extract: Specified unsupported extension (" << ext << ") for "
             << "output file" << endl;
        exit(1);
    }
    if ((pointsPath == "") && (pointsWriter->getType() == Writer::HDF_WRITER))
    {
        pointsPath = "/points";
    }

    /* gather up expected command line arguments */
    meshPartReader.load_args(opt, "mesh");
    quadratureReader.load_args(opt, "quadrature");

    /* validate these arguments and complain about missing ones */
    meshPartReader.validate_args("extract");
    quadratureReader.validate_args("extract");


    /* load mesh into memory */
    meshPartReader.load_mesh();
    if (meshPartReader.meshPart == 0)
    {
        if (!meshPartReader.has_paths())
        {
            cerr << "extract: Missing input option --mesh" << endl;
            exit(1);
        }
        else
        {
            cerr << "extract: Invalid mesh options! Could not load mesh." << endl;
            exit(1);
        }
    }
    meshPart = meshPartReader.meshPart;
    assert(meshPart != 0);


    /* now, load quadrature rule */
    quadratureReader.verbose = true;
    quadratureReader.set_mesh(meshPart);
    quadratureReader.load_quadrature();
    if (quadratureReader.quadrature == 0)
    {
        cerr << "extract: Invalid quadrature rule options!" << endl;
        exit(1);
    }
    quadrature = quadratureReader.quadrature;
    assert(quadrature != 0);


    /* finally, create field over the coordinates */
    coordsField = new FE_Field();
    coordsField->set_fe(quadrature);

    /* done with configure() step */
    return;
}


// ----------------------------------------------------------------------------

int ExtractCmd::run()
{
    assert(coordsField != 0);

    // 
    // local data -- destructure field object
    //

    QuadratureRule *fe = coordsField->fe;
    assert(fe != 0);

    QuadraturePoints *qpts = fe->points;
    assert(qpts != 0);

    MeshPart *meshPart = fe->meshPart;
    assert(meshPart != 0);

    Cell *cell = fe->meshPart->cell;
    assert(cell != 0);


    //
    // indices
    //
    int e,q;
    int i,n;

    //
    // dimensions
    //
    int nel = meshPart->nel;
    int nsd = meshPart->nsd;
    int nq  = qpts->n_points();

    double *points = new double[nel * nq * nsd]; // XXX: handle OOM

    if (verbose)
    {
        cout << "Extracting " << (nel * nq) << " points from input mesh..." << endl;
    }


    for (e = 0; e < nel; e++)
    {
        meshPart->select_cell(e);
        qpts->apply_refmap(cell);
        for (q = 0; q < nq; q++)
        {
            for (i = 0; i < nsd; i++)
            {
                n = (nq*nsd)*e + nsd*q + i;
                points[n] = qpts->data[nsd*q + i];
            }
        }
    }


    int status;

    cout << "Creating file " << outputFilename << endl;

    status = pointsWriter->open(outputFilename.c_str());
    if (status < 0)
    {
        cerr << "extract: Could not open (or create) the output file " << outputFilename << endl;
        exit(1);
    }

    status = pointsWriter->write_coordinates(pointsPath.c_str(), points, nq*nel, nsd);
    if (status < 0)
    {
        cerr << "extract: Could not write points dataset " << pointsPath << endl;
        exit(1);
    }
    
    status = pointsWriter->close();

    delete [] points;    // XXX: use auto_ptr?

    return 0;
}

// ----------------------------------------------------------------------------
