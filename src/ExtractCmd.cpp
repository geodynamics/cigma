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
    opt->addUsage("     --rule                  Quadrature rule");
    opt->addUsage("     --output                Output file");

    /* setup flags and options */

    opt->setFlag("help", 'h');
    opt->setFlag("verbose", 'v');

    opt->setOption("mesh");
    opt->setOption("mesh-coordinates");
    opt->setOption("mesh-connectivity");

    opt->setOption("rule", 'r');
    opt->setOption("rule-order");
    opt->setOption("rule-points");
    opt->setOption("rule-weights");

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
    quadratureReader.load_args(opt, "rule");

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

    coordsField = new FE_Field();
    coordsField->meshPart = meshPartReader.meshPart;
    coordsField->meshPart->set_cell();

    /* now, load quadrature rule */
    quadratureReader.load_quadrature(coordsField->meshPart->cell);
    if (quadratureReader.quadrature == 0)
    {
        cerr << "extract: Invalid quadrature rule options!" << endl;
        exit(1);
    }

    /* set up interpolator on mesh coordinates */
    coordsField->fe = new FE();
    coordsField->fe->set_mesh(meshPartReader.meshPart);
    coordsField->fe->set_quadrature_points(quadratureReader.quadrature);


    return;
}


// ----------------------------------------------------------------------------

int ExtractCmd::run()
{
    assert(coordsField != 0);

    // 
    // local data -- destructure field object
    //

    FE *fe = coordsField->fe;
    assert(fe != 0);

    MeshPart *meshPart = coordsField->meshPart;
    assert(meshPart != 0);

    QuadraturePoints *qpts = fe->points;
    assert(qpts != 0);

    Cell *cell = fe->meshPart->cell;
    assert(cell != 0);


    //
    // indices
    //
    int e,q;

    //
    // dimensions
    //
    int nel = meshPart->nel;
    int nsd = meshPart->nsd;
    int nq  = qpts->n_points();

    const int cell_nno = cell->n_nodes();

    double *globalPoints = new double[(nq*nel) * nsd]; // XXX: handle OOM

    if (verbose)
    {
        cout << "Extracting " << (nq * nel) << " points from input mesh..." << endl;
    }

    for (e = 0; e < nel; e++)
    {
        double globalCellCoords[cell_nno * 3];
        meshPart->get_cell_coords(e, globalCellCoords);
        cell->set_global_vertices(globalCellCoords, cell_nno, 3);
        qpts->apply_refmap(cell);

        for (q = 0; q < nq; q++)
        {
            for (int i = 0; i < nsd; i++)
            {
                int n = (nq*nsd)*e + nsd*q + i;
                globalPoints[n] = (*qpts)(q,i);
            }
        }
    }

    int ierr;

    cout << "Creating file " << outputFilename << endl;

    ierr = pointsWriter->open(outputFilename.c_str());
    if (ierr < 0)
    {
        cerr << "extract: Could not open (or create) the output file " << outputFilename << endl;
        exit(1);
    }

    ierr = pointsWriter->write_coordinates(pointsPath.c_str(), globalPoints, nq*nel, nsd);
    if (ierr < 0)
    {
        cerr << "extract: Could not write points dataset " << pointsPath << endl;
        exit(1);
    }
    
    ierr = pointsWriter->close();

    delete [] globalPoints;    // XXX: use auto_ptr?

    return 0;
}

// ----------------------------------------------------------------------------
