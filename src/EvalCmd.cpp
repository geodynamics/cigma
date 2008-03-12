#include <iostream>
#include <cassert>
#include "EvalCmd.h"
#include "StringUtils.h"

#include "HdfReader.h"
#include "VtkReader.h"
#include "TextReader.h"

#include "HdfWriter.h"
#include "VtkWriter.h"
#include "TextWriter.h"

#include "MeshPart.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

cigma::EvalCmd::EvalCmd()
{
    name = "eval";
    field = 0;
    points = 0;
    values = 0;
}

cigma::EvalCmd::~EvalCmd()
{
}

// ---------------------------------------------------------------------------

void cigma::EvalCmd::setupOptions(AnyOption *opt)
{
    //cout << "Calling cigma::EvalCmd::setupOptions()" << endl;
    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("");
    opt->addUsage("   cigma eval [args ...]");
    opt->addUsage("       --points      Input points to evaluate on");
    opt->addUsage("       --field       Input field to evaluate");
    opt->addUsage("       --values      Target path for result of evaluations");

    /* setup flags and options */
    opt->setFlag("help", 'h');
    opt->setFlag("verbose", 'v');

    opt->setOption("field");
    opt->setOption("field-mesh");
    opt->setOption("field-mesh-coordinates");
    opt->setOption("field-mesh-connectivity");

    opt->setOption("points");
    opt->setOption("values");
    return;
}


void cigma::EvalCmd::configure(AnyOption *opt)
{
    //cout << "Calling cigma::EvalCmd::configure()" << endl;
    assert(opt != 0);

    string inputstr;
    char *in;


    // read verbose flag
    verbose = opt->getFlag("verbose");


    /* determine the output values path */
    in = opt->getValue("values");
    if (in == 0)
    {
        cerr << "eval: Please specify the output option --values" << endl;
        exit(1);
    }
    inputstr = in;

    // determine the extension and instantiate the appropriate writer object
    string values_ext;
    parse_dataset_path(inputstr, values_loc, values_file, values_ext);
    new_writer(&values_writer, values_ext);
    if (values_writer == 0)
    {
        cerr << "eval: Specified a bad extension (" << values_ext << ") for "
             << "output file" << values_file << endl;
        exit(1);
    }
    if ((values_loc == "") && (values_writer->getType() == Writer::HDF_WRITER))
    {
        values_loc = "/values";
    }


    /* determine the input points path */
    in = opt->getValue("points");
    if (in == 0)
    {
        cerr << "eval: Please specify the input option --points" << endl;
        exit(1);
    }
    inputstr = in;

    // determine the extension and instantiate the appropriate reader object
    string points_ext;
    parse_dataset_path(inputstr, points_loc, points_file, points_ext);
    new_reader(&points_reader, points_ext);
    if (points_reader == 0)
    {
        cerr << "eval: Specified a bad extension (" << points_ext << ") for "
             << "input file" << points_file << endl;
        exit(1);
    }
    if ((points_loc == "") && (points_reader->getType() == Reader::HDF_READER))
    {
        points_loc = "/points";
    }


    /* determine the input field */
    load_args(opt, &fieldIO, "field");
    validate_args(&fieldIO, "eval");
    fieldIO.load();
    field = fieldIO.field;
    assert(field != 0);
    cout << "field path = " << fieldIO.field_path << endl;
    cout << "field rank = " << field->n_rank() << endl;
    //cout << (*field) << endl;

    return;
}

int cigma::EvalCmd::run()
{
    //cout << "Calling cigma::EvalCmd::run()" << endl;
    assert(field != 0);
    assert(points != 0);
    assert(values != 0);
    assert(field->n_dim() == points->n_dim());

    // indices
    int e,i;

    // dimensions
    int npts = points->n_points();
    int valdim = field->n_rank();

    // data
    double *phi = new double[npts * valdim];


    for (i = 0; i < npts; i++)
    {
        double *globalPoint = (*points)[i];
        field->eval(globalPoint, &phi[valdim*i]);
    }



    /*
    for (i = 0; i < npts; i++)
    {
        double *globalPoint = (*points)[i];
        field->meshPart->find_cell(globalPoint, &e);
        field->meshPart->get_cell_coords(e, field->fe->cell->globverts);
        field->eval(globalPoint, &phi[valdim*i]);
    }

    std::cout << "Creating file " << output_filename << std::endl;
    VtkWriter *writer = new VtkWriter();
    writer->open(output_filename);
    writer->write_header();
    writer->write_point_data("values", phi, npts, valdim);
    writer->close();
    //delete writer;
    */



    int ierr;

    cout << "Creating file " << values_file << endl;

    if (values_writer->getType() == Writer::HDF_WRITER)
    {
        HdfWriter *writer = static_cast<HdfWriter*>(values_writer);
        ierr = writer->open(values_file);
        if (ierr < 0)
        {
            cerr << "Error: Could not open (or create) the HDF5 file " << values_file << endl;
            exit(1);
        }

        ierr = writer->write_coordinates(values_loc.c_str(), phi, npts, valdim);
        if (ierr < 0)
        {
            cerr << "Error: Could not write values dataset " << values_loc << endl;
            exit(1);
        }
        writer->close();
    }
    else if (values_writer->getType() == Writer::TXT_WRITER)
    {
        TextWriter *writer = static_cast<TextWriter*>(values_writer);
        ierr = writer->open(values_file);
        if (ierr < 0)
        {
            cerr << "Error: Could not create output text file " << values_file << endl;
            exit(1);
        }
        writer->write_coordinates(phi, npts, valdim);
    }
    else if (values_writer->getType() == Writer::VTK_WRITER)
    {
        VtkWriter *writer = static_cast<VtkWriter*>(writer);
        ierr = vtkWriter->open(values_file);
        if (ierr < 0)
        {
            cerr << "Error: Could not create output VTK file " << values_file << endl;
            exit(1);
        }
        writer->write_header();
        //writer->write_points(...);
        writer->write_point_data("values", phi, npts, valdim);
        writer->close();
    }
    else
    {
        /* this should be unreachable */
        cerr << "Fatal Error: Unsupported extension in output filename?" << endl;
        return 1;
    }

    // XXX: wrap this guy inside an auto_ptr
    delete [] phi;

    return 0;
}
