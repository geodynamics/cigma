#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string>
#include "../AnyOption.h"
#include "../StringUtils.h"
#include "../TextWriter.h"
#include "../HdfReader.h"

using namespace std;
using namespace cigma;

int main(int argc, char *argv[])
{
    AnyOption *opt = new AnyOption();
    const char *in;
    
    string inputstr;
    string ext;

    string filename;
    string coords_file;
    string connect_file;
    string field_file;

    string coords_loc;
    string connect_loc;
    string field_loc;

    string field_name;


    opt->setFlag("help", 'h');
    opt->setOption("input");
    opt->setOption("coords");
    opt->setOption("connect");
    opt->setOption("field");

    opt->addUsage("Usage:");
    opt->addUsage("    TestHdfReader [options]");
    opt->addUsage("      --help         ");
    opt->addUsage("      --input        Input file");
    opt->addUsage("      --coords       Location of mesh coordinates");
    opt->addUsage("      --connect      Location of mesh connectivity");
    opt->addUsage("      --field        Location of field data");

    opt->processCommandArgs(argc, argv);

    /*
    if (!opt->hasOptions())
    {
        opt->printUsage();
        exit(1);
    } // */

    if (opt->getFlag("help"))
    {
        opt->printUsage();
        exit(0);
    }


    const bool debug = true;


    in = opt->getValue("input");
    if (in == 0)
    {
        in = "/home/luis/benchmarks/bmssnog.h5";
        if (!debug)
        {
            cerr << "Please specify the option --input" << endl;
            exit(1);
        }
    }
    filename = in;


    in = opt->getValue("coords");
    if (in == 0)
    {
        in = "/mesh/bmssnog_tet4_0250m/coordinates";
        if (!debug)
        {
            cerr << "Please specify the option --coords" << endl;
            exit(1);
        }
    }
    coords_loc = in;
    //inputstr = in;
    //parse_dataset_path(inputstr, coords_loc, coords_file, ext);

    in = opt->getValue("connect");
    if (in == 0)
    {
        in = "/mesh/bmssnog_tet4_0250m/connectivity";
        if (!debug)
        {
            cerr << "Please specify the option --connect" << endl;
            exit(1);
        }
    }
    connect_loc = in;
    //inputstr = in;
    //parse_dataset_path(inputstr, connect_loc, connect_file, ext);

    in = opt->getValue("field");
    if (in == 0)
    {
        in = "/geofest/bmssnog_tet4_0250m/variables/displacement/step00000";
        if (!debug)
        {
            cerr << "Please specify the option --field" << endl;
            exit(1);
        }
    }
    field_loc = in;
    //inputstr = in;
    //parse_dataset_path(inputstr, field_loc, field_file, ext);


    TextWriter *writer = new TextWriter();
    writer->fp = stdout;

    HdfReader *reader = new HdfReader();


    reader->open(filename);

    reader->set_coordinates_path(coords_loc.c_str());
    reader->set_connectivity_path(connect_loc.c_str());
    reader->set_dataset_path(field_loc.c_str());

    int nno, nsd;
    double *coords;
    reader->get_coordinates(&coords, &nno, &nsd);

    int nel, ndofs;
    int *connect;
    reader->get_connectivity(&connect, &nel, &ndofs);


    int dset_dims[2];
    double *dset_data;
    reader->get_dataset(&dset_data, &dset_dims[0], &dset_dims[1]);
    
    cout << "Reading " << filename << endl;
    cout << "Coordinates: " << coords_loc << " (" << nno << " x " << nsd << ")" << endl;
    cout << "Connectivity: " << connect_loc << " (" << nel << " x " << ndofs << ")" << endl;
    cout << "Field: " << field_loc << " (" << dset_dims[0] << " x " << dset_dims[1] << ")" << endl;


    sleep(100);


    delete reader;

    writer->fp = NULL;
    delete writer;
}
