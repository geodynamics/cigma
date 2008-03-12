#include <iostream>
#include <string>
#include <cassert>

#include "CubeCmd.h"
#include "HdfWriter.h"
#include "TextWriter.h"
#include "VtkWriter.h"
#include "StringUtils.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

cigma::CubeCmd::CubeCmd()
{
    name = "cube";

    L = M = N = 0;
    mesh = 0;
    writer = 0;
}

cigma::CubeCmd::~CubeCmd()
{
}


// ---------------------------------------------------------------------------

void cigma::CubeCmd::setupOptions(AnyOption *opt)
{
    //cout << "Calling cigma::CubeCmd::setupOptions()" << endl;
    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("  cigma cube [options]");
    opt->addUsage("    -x               Number of elements along x-dimension");
    opt->addUsage("    -y               Number of elements along y-dimension");
    opt->addUsage("    -z               Number of elements along z-dimension");
    opt->addUsage("    --hex8           Create hexahedral partition (default)");
    opt->addUsage("    --tet4           Create tetrahedral partition");
    opt->addUsage("    --output         Target output file");
    opt->addUsage("    --coords-path    Target path for coordinates (.h5 only)");
    opt->addUsage("    --connect-path   Target path for connectivity (.h5 only)");

    /* setup flags and options */

    opt->setFlag("help", 'h');
    opt->setFlag("verbose", 'v');
    
    opt->setFlag("hex8");
    opt->setFlag("tet4");
    
    opt->setOption('x');
    opt->setOption('y');
    opt->setOption('z');

    opt->setOption("output");
    opt->setOption("coords-path");
    opt->setOption("connect-path");

}


void cigma::CubeCmd::configure(AnyOption *opt)
{
    //std::cout << "Calling cigma::CubeCmd::configure()" << std::endl;
    assert(opt != 0);

    if (!opt->hasOptions())
    {
        //std::cerr << "No options?" << std::endl;
        opt->printUsage();
        exit(1);
    }

    char *in;
    string inputstr;


    // read verbose flag
    verbose = opt->getFlag("verbose");


    // read L
    in = opt->getValue('x');
    if (in == 0)
    {
        cerr << "cube: Please specify the option -x" << endl;
        exit(1);
    }
    inputstr = in;
    string_to_int(inputstr, L);

    // read M
    in = opt->getValue('y');
    if (in == 0)
    {
        cerr << "cube: Please specify the option -y" << endl;
        exit(1);
    }
    inputstr = in;
    string_to_int(inputstr, M);

    // read N
    in = opt->getValue('z');
    if (in == 0)
    {
        cerr << "cube: Please specify the option -z" << endl;
        exit(1);
    }
    inputstr = in;
    string_to_int(inputstr, N);



    // read output file
    in = opt->getValue("output");
    if (in == 0)
    {
        cerr << "cube: Please specify the option --output" << endl;
        exit(1);
    }
    output_filename = in;


    // determine the extension and instantiate appropriate writer object
    string root, ext;
    path_splitext(output_filename, root, ext);
    new_writer(&writer, ext);
    if (writer == 0)
    {
        cerr << "cube: File with bad extension (" << ext << ")" << endl;
        exit(1);
    }


    // read target path for coordinates array
    in = opt->getValue("coords-path");
    if (in == 0)
    {
        if (writer->getType() == Writer::HDF_WRITER)
        {
            coords_path = "/coordinates";
        }
    }
    else
    {
        coords_path = in;
    }
    if ((coords_path != "") && (writer->getType() != Writer::HDF_WRITER))
    {
        cerr << "cube: Can only use --coords-path "
             << "when writing to an HDF5 (.h5) file" << endl;
        exit(1);

    }

    // read target path for connectivity array
    in = opt->getValue("connect-path");
    if (in == 0)
    {
        if (writer->getType() == Writer::HDF_WRITER)
        {
            connect_path = "/connectivity";
        }
    }
    else
    {
        connect_path = in;
    }
    if ((connect_path != "") && (writer->getType() != Writer::HDF_WRITER))
    {
        cerr << "cube: Can only use --connect-path "
             << "when writing to an HDF5 (.h5) file" << endl;
        exit(1);
    }


    // read tet/hex flags
    bool hexFlag = opt->getFlag("hex8");
    bool tetFlag = opt->getFlag("tet4");
    if (hexFlag && tetFlag)
    {
        std::cerr << "cube: Please specify only one of the flags "
                  << "--hex8 or --tet8" << std::endl;
        exit(1);
    }
    if (!tetFlag)
    {
        hexFlag = true;
    }
    assert(hexFlag != tetFlag);


    // initialize mesh object
    mesh = new cigma::CubeMeshPart();
    mesh->calc_coordinates(L,M,N);
    if (hexFlag) { mesh->calc_hex8_connectivity(); }
    if (tetFlag) { mesh->calc_tet4_connectivity(); }
}


int cigma::CubeCmd::run()
{
    //std::cout << "Calling cigma::CubeCmd::run()" << std::endl;
    assert(mesh != 0);
    assert(writer != 0);

    if (verbose)
    {
        std::cout << "L, M, N = (" << L << ", " << M << ", " << N << ")" << std::endl;
        std::cout << "mesh->nno = " << mesh->nno << std::endl;
        std::cout << "mesh->nel = " << mesh->nel << std::endl;
        std::cout << "mesh->ndofs = " << mesh->ndofs << std::endl;
    }


    if (verbose)
    {
        int e = 0;
        double pts[2][3] = {{0.5, 0.5, 0.5},
                            {1.0, 1.0, 1.0}};

        cout << "Looking for centroid..." << endl;
        bool found = mesh->find_cell(pts[0], &e);

        if (!found)
        {
            cerr << "Error: Could not find cell "
                 << "containing centroid (0.5,0.5,0.5)" << endl;
            exit(1);
        }
        else
        {
            cout << "Found centroid in cell " << e << endl;
        }
    }

    int ierr;

    if (writer->getType() == Writer::HDF_WRITER)
    {
        cout << "Creating file " << output_filename << endl;

        HdfWriter *hdfWriter = static_cast<HdfWriter*>(writer);
        ierr = hdfWriter->open(output_filename);
        if (ierr < 0)
        {
            cerr << "Error: Could not open (or create) the HDF5 file " << output_filename << endl;
            exit(1);
        }

        ierr = hdfWriter->write_coordinates(coords_path.c_str(), mesh->coords, mesh->nno, mesh->nsd);
        if (ierr < 0)
        {
            cerr << "Error: Could not write dataset " << coords_path << endl;
            exit(1);
        }

        ierr = hdfWriter->write_connectivity(connect_path.c_str(), mesh->connect, mesh->nel, mesh->ndofs);
        if (ierr < 0)
        {
            cerr << "Error: Could not write dataset " << connect_path << endl;
            exit(1);
        }

        hdfWriter->close();
    }
    else if (writer->getType() == Writer::TXT_WRITER)
    {
        cout << "Creating file " << output_filename << endl;

        TextWriter *txtWriter = static_cast<TextWriter*>(writer);
        ierr = txtWriter->open(output_filename);
        if (ierr < 0)
        {
            cerr << "Error: Could not create output text file " << output_filename << endl;
            exit(1);
        }

        txtWriter->write_coordinates(mesh->coords, mesh->nno, mesh->nsd);
        txtWriter->write_connectivity(mesh->connect, mesh->nel, mesh->ndofs);
        txtWriter->close();
    }
    else if (writer->getType() == Writer::VTK_WRITER)
    {
        cout << "Creating file " << output_filename << endl;

        VtkWriter *vtkWriter = static_cast<VtkWriter*>(writer);
        ierr = vtkWriter->open(output_filename);
        if (ierr < 0)
        {
            cerr << "Error: Could not create output VTK file " << output_filename << endl;
            exit(1);
        }

        vtkWriter->write_header();
        vtkWriter->write_points(mesh->coords, mesh->nno, mesh->nsd);
        vtkWriter->write_cells(mesh->connect, mesh->nel, mesh->ndofs);
        vtkWriter->write_cell_types(mesh->nsd, mesh->nel, mesh->ndofs);
        vtkWriter->close();
    }
    else
    {
        cerr << "cube: File with bad extension?" << endl;
        return 1;
    }


    if (writer != 0)
    {
        delete writer;
    }

    return 0;
}

// ---------------------------------------------------------------------------

