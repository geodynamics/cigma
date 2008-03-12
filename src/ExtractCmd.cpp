#include <iostream>
#include <cassert>
#include "ExtractCmd.h"
#include "StringUtils.h"
#include "HdfWriter.h"
#include "TextWriter.h"
#include "VtkWriter.h"
#include "MeshPart.h"


using namespace std;
using namespace cigma;


// ---------------------------------------------------------------------------

cigma::ExtractCmd::ExtractCmd()
{
    name = "extract";
    coordsField = 0;
    writer = 0;
}

cigma::ExtractCmd::~ExtractCmd()
{
}


// ---------------------------------------------------------------------------

void cigma::ExtractCmd::setupOptions(AnyOption *opt)
{
    //std::cout << "Calling cigma::ExtractCmd::setupOptions()" << std::endl;
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


void cigma::ExtractCmd::configure(AnyOption *opt)
{
    //std::cout << "Calling cigma::ExtractCmd::configure()" << std::endl;
    assert(opt != 0);


    string inputstr;
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


    // determine the extension and instantiate the appropriate writer object
    string ext;
    parse_dataset_path(inputstr, points_path, output_filename, ext);
    new_writer(&writer, ext);
    if (writer == 0)
    {
        cerr << "extract: Specified bad extension (" << ext << ") for "
             << "output file" << endl;
        exit(1);
    }
    if ((points_path == "") && (writer->getType() == Writer::HDF_WRITER))
    {
        points_path = "/points";
    }


    /* gather up expected command line arguments */
    load_args(opt, &meshIO, "mesh");
    load_args(opt, &quadratureIO, "rule");

    /* validate these arguments and complain about missing ones */
    validate_args(&meshIO, "extract");
    validate_args(&quadratureIO, "extract");

    /* create empty field object */
    coordsField = new FE_Field();

    /* load mesh into memory */
    meshIO.load();
    if (meshIO.meshPart == 0)
    {
        if (!meshIO.has_paths())
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
    coordsField->meshPart = meshIO.meshPart;
    coordsField->meshPart->set_cell();


    /* now, load quadrature rule */
    quadratureIO.load(coordsField->meshPart->cell);
    if (quadratureIO.quadrature == 0)
    {
        cerr << "extract: Invalid quadrature rule options!" << endl;
        exit(1);
    }


    /* finite element for interpolation on mesh coordinates */
    coordsField->fe = new FE();
    coordsField->fe->set_mesh(meshIO.meshPart);
    coordsField->fe->set_quadrature_points(quadratureIO.quadrature);


    return;
}


int cigma::ExtractCmd::run()
{
    //std::cout << "Calling cigma::ExtractCmd::run()" << std::endl;
    assert(coordsField != 0);


    // local data -- destructure field object

    FE *fe = coordsField->fe;
    assert(fe != 0);

    MeshPart *meshPart = coordsField->meshPart;
    assert(meshPart != 0);

    QuadraturePoints *quadrature = fe->points;
    assert(quadrature != 0);

    Cell *cell = fe->meshPart->cell;
    assert(cell != 0);


    // indices
    int e,q;


    // dimensions
    int nel = meshPart->nel;
    int nsd = meshPart->nsd;
    int nq = quadrature->n_points();

    const int cell_nno = cell->n_nodes();

    double *global_points = new double[(nq*nel) * nsd];


    if (verbose)
    {
        cout << "Extracting "
             << (nq * nel)
             << " points from input mesh..."
             << endl;
    }

    for (e = 0; e < nel; e++)
    {
        double globalCellCoords[cell_nno*3];
        meshPart->get_cell_coords(e, globalCellCoords);
        cell->set_global_vertices(globalCellCoords, cell_nno, 3);
        quadrature->apply_refmap(cell);

        for (q = 0; q < nq; q++)
        {
            for (int i = 0; i < nsd; i++)
            {
                int n = (nq*nsd)*e + nsd*q + i;
                global_points[n] = (*quadrature)(q,i);
            }
        }
        /*
        for (q = 0; q < nq; q++)
        {
            std::cout << (*quadrature)(q,0) << " "
                      << (*quadrature)(q,1) << " "
                      << (*quadrature)(q,2) << std::endl;
        } // */
    }


    int ierr;

    cout << "Creating file " << output_filename << endl;

    if (writer->getType() == Writer::HDF_WRITER)
    {
        HdfWriter *hdfWriter = static_cast<HdfWriter*>(writer);
        ierr = hdfWriter->open(output_filename);
        if (ierr < 0)
        {
            cerr << "Error: Could not open (or create) the HDF5 file " << output_filename << endl;
            exit(1);
        }

        ierr = hdfWriter->write_coordinates(points_path.c_str(), global_points, nq*nel, nsd);
        if (ierr < 0)
        {
            cerr << "Error: Could not write points dataset " << points_path << endl;
            exit(1);
        }

        hdfWriter->close();
    }
    else if (writer->getType() == Writer::TXT_WRITER)
    {
        TextWriter *txtWriter = static_cast<TextWriter*>(writer);
        ierr = txtWriter->open(output_filename);
        if (ierr < 0)
        {
            cerr << "Error: Could not create output text file " << output_filename << endl;
            exit(1);
        }

        txtWriter->write_coordinates(global_points, nq * nel, nsd);
        txtWriter->close();
    }
    else if (writer->getType() == Writer::VTK_WRITER)
    {
        VtkWriter *vtkWriter = static_cast<VtkWriter*>(writer);
        ierr = vtkWriter->open(output_filename);
        if (ierr < 0)
        {
            cerr << "Error: Could not create output VTK file " << output_filename << endl;
            exit(1);
        }

        vtkWriter->write_header();
        vtkWriter->write_points(global_points, nq * nel, nsd);
        vtkWriter->close();
    }
    else
    {
        /* this should be unreachable */
        cerr << "Fatal Error: Unsupported extension in output filename?" << endl;
        return 1;
    }

    
    // XXX: wrap this guy inside an auto_ptr
    delete [] global_points;


    return 0;
}
