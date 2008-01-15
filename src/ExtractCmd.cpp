#include <iostream>
#include <cassert>
#include "ExtractCmd.h"
#include "StringUtils.h"
#include "VtkUgReader.h"
#include "VtkUgMeshPart.h"
#include "VtkUgSimpleWriter.h"
#include "Tet.h"
#include "Hex.h"

// ---------------------------------------------------------------------------

cigma::ExtractCmd::ExtractCmd()
{
    name = "extract";
    coords = 0;
}

cigma::ExtractCmd::~ExtractCmd()
{
}

// ---------------------------------------------------------------------------

void cigma::ExtractCmd::setupOptions(AnyOption *opt)
{
    std::cout << "Calling cigma::ExtractCmd::setupOptions()" << std::endl;
    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("   cigma extract [options]");
    opt->addUsage("     --help          Display this screen");
    opt->addUsage("     --order         Quadrature order");
    opt->addUsage("     --meshfile      Mesh file");
    opt->addUsage("     --output        Output file");
    //opt->addUsage("     --meshpath      Mesh path");

    /* setup flags and options */
    opt->setFlag("help", 'h');
    opt->setOption("order", 'o');
    opt->setOption("meshfile");
    //opt->setOption("meshpath");
    opt->setOption("output");
}


void cigma::ExtractCmd::configure(AnyOption *opt)
{
    std::cout << "Calling cigma::ExtractCmd::configure()" << std::endl;
    assert(opt != 0);


    coords = new FE_Field();


    std::string meshfile, meshpath;
    std::string root, ext;
    char *in;

    in = opt->getValue("meshfile");
    if (in == 0)
    {
        std::cerr << "extract: Please specify the option --meshfile" << std::endl;
        exit(1);
    }
    meshfile = in;
    path_splitext(meshfile, root, ext);
    std::cout << "Parameter meshfile is " << meshfile << std::endl;
    std::cout << "meshfile root is " << root << std::endl;
    std::cout << "meshfile ext is " << ext << std::endl;

    if (ext == ".vtk")
    {
        int nno, nsd;
        int nel, ndofs;
        double *coordinates;
        int *connectivity;

        std::cout << "Reading file " << meshfile << std::endl;

        VtkUgReader reader;
        reader.open(meshfile);
        reader.get_coordinates(&coordinates, &nno, &nsd);
        reader.get_connectivity(&connectivity, &nel, &ndofs);
        std::cout << "mesh ndofs = " << ndofs << std::endl;

        coords->meshPart = new VtkUgMeshPart();
        coords->meshPart->set_coordinates(coordinates, nno, nsd);
        coords->meshPart->set_connectivity(connectivity, nel, ndofs);

        switch (ndofs)
        {
        case 4:
            coords->fe = new FE();
            coords->fe->cell = new Tet();
            {
                coords->fe->quadrature = new Quadrature();
                double tet_qpts[8*3] = {
                    -0.68663473, -0.72789005, -0.75497035,
                    -0.83720867, -0.85864055,  0.08830369,
                    -0.86832263,  0.13186633, -0.75497035,
                    -0.93159441, -0.4120024 ,  0.08830369,
                     0.16949513, -0.72789005, -0.75497035,
                    -0.39245447, -0.85864055,  0.08830369,
                    -0.50857335,  0.13186633, -0.75497035,
                    -0.74470688, -0.4120024 ,  0.08830369 };
                double tet_qwts[8] = {
                    0.29583885,  0.12821632,  0.16925605,  0.07335544,  0.29583885,
                    0.12821632,  0.16925605,  0.07335544 };
                coords->fe->quadrature->set_quadrature(tet_qpts, tet_qwts, 8, 3);
                coords->fe->quadrature->set_globaldim(3);
            }
            break;
        case 8:
            coords->fe = new FE();
            coords->fe->cell = new Hex();
            {
                coords->fe->quadrature = new Quadrature();
                double hex_qpts[8*3] = {
                    -0.57735027, -0.57735027, -0.57735027,
                     0.57735027, -0.57735027, -0.57735027,
                     0.57735027,  0.57735027, -0.57735027,
                    -0.57735027,  0.57735027, -0.57735027,
                    -0.57735027, -0.57735027,  0.57735027,
                     0.57735027, -0.57735027,  0.57735027,
                     0.57735027,  0.57735027,  0.57735027,
                    -0.57735027,  0.57735027,  0.57735027 };
                double hex_qwts[8] = { 1.,  1.,  1.,  1.,  1.,  1.,  1.,  1. };
                coords->fe->quadrature->set_quadrature(hex_qpts, hex_qwts, 8, 3);
                coords->fe->quadrature->set_globaldim(3);
            }
            break;
        }
        assert(coords->fe != 0);
        assert(coords->fe->cell != 0);

        coords->dofHandler = new DofHandler();
        coords->dofHandler->meshPart = coords->meshPart;
        coords->dofHandler->set_data(coords->meshPart->coords, nno, nsd);
    }
    else
    {
        assert(ext != ".vtk");
    }


    int order;
    std::string quadrature_order;
    in = opt->getValue("order");
    quadrature_order = (in != 0) ? in : "2";
    string_to_int(quadrature_order, order);
    std::cout << "Quadrature order is " << order << std::endl;
    assert(coords->fe->quadrature != 0);
    // coords->fe->quadrature->initialize(order);

    in = opt->getValue("output");
    if (in == 0)
    {
        std::cout << "extract: Please specify the option --output" << std::endl;
        exit(1);
    }
    output_filename = in;

    return;
}


int cigma::ExtractCmd::run()
{
    std::cout << "Calling cigma::ExtractCmd::run()" << std::endl;
    assert(coords != 0);

    // indices
    int e,q;

    // local data
    FE *fe = coords->fe;
    assert(fe != 0);
    MeshPart *meshPart = coords->meshPart;
    assert(meshPart != 0);
    Quadrature *quadrature = fe->quadrature;
    assert(quadrature != 0);
    Cell *cell = fe->cell;
    assert(cell != 0);

    // dimensions
    int nel = meshPart->nel;
    int nsd = meshPart->nsd;
    int nq = quadrature->n_points();

    const int cell_nno = cell->n_nodes();

    double *global_points = new double[nel*nq*nsd];

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

    std::cout << "Creating file " << output_filename << std::endl;
    VtkUgSimpleWriter *writer = new VtkUgSimpleWriter();
    writer->open(output_filename);
    writer->write_header();
    writer->write_points(global_points, nel*nq, nsd);
    writer->close();
    //delete writer;

    delete [] global_points;

    return 0;
}
