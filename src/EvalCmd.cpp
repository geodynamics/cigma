#include <iostream>
#include <cassert>
#include "EvalCmd.h"
#include "StringUtils.h"
#include "VtkReader.h"
#include "VtkWriter.h"
#include "MeshPart.h"
#include "Tet.h"
#include "Hex.h"

// ---------------------------------------------------------------------------

cigma::EvalCmd::EvalCmd()
{
    name = "eval";
    points = 0;
    field = 0;
}

cigma::EvalCmd::~EvalCmd()
{
}

// ---------------------------------------------------------------------------

void cigma::EvalCmd::setupOptions(AnyOption *opt)
{
    std::cout << "Calling cigma::EvalCmd::setupOptions()" << std::endl;
    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("");
    opt->addUsage("   cigma eval [args ...]");
    opt->addUsage("       --field       Field path");
    opt->addUsage("       --points      List of points");
    opt->addUsage("       --output      Output file");

    /* setup flags and options */
    opt->setFlag("help", 'h');

    opt->setOption("field");
    opt->setOption("points");
    opt->setOption("output");
    return;
}

void cigma::EvalCmd::configure(AnyOption *opt)
{
    using namespace cigma;

    std::cout << "Calling cigma::EvalCmd::configure()" << std::endl;

    std::string input, location, inputfile, ext;
    std::string pointsfile;
    char *in;

    in = opt->getValue("field");
    if (in == 0)
    {
        std::cerr << "eval: Please specify the option --field" << std::endl;
        exit(1);
    }
    input = in;
    parse_dataset_path(input, location, inputfile, ext);
    std::cout << "field location = " << location << std::endl;
    std::cout << "field inputfile = " << inputfile << std::endl;
    std::cout << "field extension = " << ext << std::endl;

    in = opt->getValue("points");
    if (in == 0)
    {
        std::cerr << "eval: Please specify the option --points" << std::endl;
        exit(1);
    }
    pointsfile = in;

    in = opt->getValue("output");
    if (in == 0)
    {
        std::cerr << "eval: Please specify the option --output" << std::endl;
        exit(1);
    }
    output_filename = in;

    int nno, nsd;
    int nel, ndofs;
    double *coords;
    int *connect;
    int dofs_nno, dofs_valdim;
    double *dofs;

    VtkReader *reader = new VtkReader();
    reader->open(inputfile);
    reader->get_coordinates(&coords, &nno, &nsd);
    reader->get_connectivity(&connect, &nel, &ndofs);
    reader->get_scalar_point_data(location.c_str(), &dofs, &dofs_nno, &dofs_valdim);

    field = new FE_Field();
    field->dim = nsd;
    field->rank = dofs_valdim;
    field->meshPart = new MeshPart();
    field->meshPart->set_coordinates(coords, nno, nsd);
    field->meshPart->set_connectivity(connect, nel, ndofs);

    switch (ndofs)
    {
    case 4:
        field->fe = new FE();
        field->fe->cell = new Tet();
        break;

    case 8:
        field->fe = new FE();
        field->fe->cell = new Hex();
        break;
    }
    assert(field->fe != 0);
    assert(field->fe->cell != 0);
    field->meshPart->cell = field->fe->cell;

    field->dofHandler = new DofHandler();
    field->dofHandler->meshPart = field->meshPart;
    field->dofHandler->set_data(dofs, dofs_nno, dofs_valdim);


    int pts_nno, pts_nsd;
    double *pts_coords;

    VtkReader *pointsreader = new VtkReader();
    pointsreader->open(pointsfile);
    pointsreader->get_coordinates(&pts_coords, &pts_nno, &pts_nsd);
    points = new Points();
    points->set_data(pts_coords, pts_nno, pts_nsd);

    return;
}

int cigma::EvalCmd::run()
{
    std::cout << "Calling cigma::EvalCmd::run()" << std::endl;
    assert(field != 0);
    assert(points != 0);
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

    return 0;
}
