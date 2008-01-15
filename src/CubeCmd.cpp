#include <iostream>
#include <cassert>
#include "CubeCmd.h"
#include "StringUtils.h"
#include "VtkUgSimpleWriter.h"

// ---------------------------------------------------------------------------

cigma::CubeCmd::CubeCmd()
{
    name = "cube";
}

cigma::CubeCmd::~CubeCmd()
{
}


// ---------------------------------------------------------------------------

void cigma::CubeCmd::setupOptions(AnyOption *opt)
{
    std::cout << "Calling cigma::CubeCmd::setupOptions()" << std::endl;
    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("  cigma cube [options]");
    opt->addUsage("    -L               Partition elements along x-dimension");
    opt->addUsage("    -M               Partition elements along y-dimension");
    opt->addUsage("    -N               Partition elements along z-dimension");
    opt->addUsage("    -o  --output     Filename for mesh output file");
    opt->addUsage("        --hex        Create hexahedral partition (default)");
    opt->addUsage("        --tet        Create tetrahedral partition");

    /* setup flags and options */

    opt->setFlag("help", 'h');
    opt->setFlag("hex");
    opt->setFlag("tet");
    opt->setOption('L');
    opt->setOption('M');
    opt->setOption('N');
    opt->setOption("output");
}

void cigma::CubeCmd::configure(AnyOption *opt)
{
    std::cout << "Calling cigma::CubeCmd::configure()" << std::endl;
    assert(opt != 0);

    char *in;
    std::string inputstr;

    // read L
    in = opt->getValue('L');
    if (in == 0)
    {
        std::cerr << "cube: Please specify the option -L" << std::endl;
        exit(1);
    }
    inputstr = in;
    string_to_int(inputstr, L);

    // read M
    in = opt->getValue('M');
    if (in == 0)
    {
        std::cerr << "cube: Please specify the option -M" << std::endl;
        exit(1);
    }
    inputstr = in;
    string_to_int(inputstr, M);

    // read N
    in = opt->getValue('N');
    if (in == 0)
    {
        std::cerr << "cube: Please specify the option -N" << std::endl;
        exit(1);
    }
    inputstr = in;
    string_to_int(inputstr, N);
    
    // read output file
    in = opt->getValue("output");
    if (in == 0)
    {
        std::cerr << "cube: Please specify the option --output" << std::endl;
        exit(1);
    }
    output_filename = in;

    // read tet/hex flags
    bool hexFlag = opt->getFlag("hex");
    bool tetFlag = opt->getFlag("tet");
    if (hexFlag && tetFlag)
    {
        std::cerr << "cube: Please specify only one of the flags "
                  << "--hex or --tet" << std::endl;
        exit(1);
    }
    if (!tetFlag)
    {
        hexFlag = true;
    }
    assert(hexFlag != tetFlag);


    // initialize mesh
    mesh = new cigma::CubeMeshPart();
    mesh->calc_coordinates(L,M,N);
    if (hexFlag) { mesh->calc_hex8_connectivity(); }
    if (tetFlag) { mesh->calc_tet4_connectivity(); }
}

int cigma::CubeCmd::run()
{
    std::cout << "Calling cigma::CubeCmd::run()" << std::endl;
    std::cout << "L, M, N = (" << L << ", " << M << ", " << N << ")" << std::endl;
    std::cout << "mesh->nno = " << mesh->nno << std::endl;
    std::cout << "mesh->nel = " << mesh->nel << std::endl;
    std::cout << "mesh->ndofs = " << mesh->ndofs << std::endl;

    int e = 0;
    double pts[2][3] = {{0.5, 0.5, 0.5},
                        {1.0, 1.0, 1.0}};

    bool found = mesh->find_cell(pts[0], &e);
    if (!found)
    {
        std::cout << "Could not find pts[0] = {0.5,0.5,0.5}!" << std::endl;
    }
    else
    {
        std::cout << "Found point in cell " << e << std::endl;
    }

    std::cout << "Creating file " << output_filename << std::endl;
    VtkUgSimpleWriter *writer = new VtkUgSimpleWriter();
    writer->open(output_filename);
    writer->write_header();
    writer->write_points(mesh->coords, mesh->nno, mesh->nsd);
    writer->write_cells(mesh->connect, mesh->nel, mesh->ndofs);
    writer->write_cell_types(mesh->nsd, mesh->nel, mesh->ndofs);
    writer->close();
    //delete writer;
}

// ---------------------------------------------------------------------------

