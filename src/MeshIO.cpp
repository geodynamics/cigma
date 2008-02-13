#include <iostream>
#include <cstdlib>
#include "MeshIO.h"
#include "StringUtils.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

void load_args(AnyOption *opt, MeshIO *meshIO, const char *opt_prefix)
{
    assert(opt != 0);
    assert(meshIO != 0);

    char *in;
    string optstr;
    string mesh_name = opt_prefix;

    in = opt->getValue(mesh_name.c_str());
    if (in != 0)
    {
        meshIO->mesh_path = in;
    }

    optstr = mesh_name + "-coordinates";
    in = opt->getValue(optstr.c_str());
    if (in != 0)
    {
        meshIO->coords_path = in;
    }

    optstr = mesh_name + "-connectivity";
    in = opt->getValue(optstr.c_str());
    if (in != 0)
    {
        meshIO->connect_path = in;
    }
}

void validate_args(MeshIO *meshIO, const char *cmd_name)
{
    assert(meshIO != 0);

    if (meshIO->has_paths())
    {
        if (meshIO->mesh_path == "")
        {
            if (meshIO->coords_path == "")
            {
                cerr << cmd_name
                     << ": Detected missing option --mesh-coordinates"
                     << endl;

                exit(1);
            }
            if (meshIO->connect_path == "")
            {
                cerr << cmd_name
                     << ": Detected missing option --mesh-connectivity"
                     << endl;

                exit(1);
            }
        }
    }
}

// ---------------------------------------------------------------------------


bool MeshIO::has_paths()
{
    return (mesh_path != "") || (coords_path != "") || (connect_path != "");
}

bool MeshIO::has_coords_path()
{
    return (mesh_path != "") || (coords_path != "");
}

bool MeshIO::has_connect_path()
{
    return (mesh_path != "") || (connect_path != "");
}

bool MeshIO::has_valid_paths()
{
    // empty state is also valid
    if (!has_paths())
    {
        return true;
    }

    // require both coords and connect paths if mesh path is absent
    return (mesh_path != "") || ((coords_path != "") && (connect_path != ""));
}

// ---------------------------------------------------------------------------

MeshIO::MeshIO()
{
    meshPart = 0;
}

MeshIO::~MeshIO()
{
    if (meshPart != 0)
    {
        // XXX: traverse meshPart structure and delete everything
    }
}

// ---------------------------------------------------------------------------

void MeshIO::load()
{
    cout << "Calling MeshIO::load()" << endl;

    // skip this method if paths are empty
    if (!has_paths())
    {
        return;
    }

    //string mesh_loc, mesh_file, mesh_ext;
    //string coords_loc, coords_file, coords_ext;
    //string connect_loc, connect_file, connect_ext;

    int nno, nsd;
    double *coords = 0;

    int nel, ndofs;
    int *connect = 0;

    nno = nsd = 0;
    nel = ndofs = 0;


    //
    // Note that each of coords_path and connect_path will override
    // the corresponding component of mesh_path
    //


    // XXX: use auto_ptr for the local readers, so we can throw exceptions
    if (coords_path != "")
    {
        Reader *coords_reader;
        parse_dataset_path(coords_path, coords_loc, coords_file, coords_ext);
        new_reader(&coords_reader, coords_ext);
        coords_reader->open(coords_file);
        coords_reader->get_coordinates(coords_loc.c_str(), &coords, &nno, &nsd);
        //coords_reader->close();
    }

    if (connect_path != "")
    {
        Reader *connect_reader;
        parse_dataset_path(connect_path, connect_loc, connect_file, connect_ext);
        new_reader(&connect_reader, connect_ext);
        connect_reader->open(connect_file);
        connect_reader->get_connectivity(connect_loc.c_str(), &connect, &nel, &ndofs);
        //connect_reader->close();
    }

    if ((mesh_path != "") && ((coords == 0) || (connect == 0)))
    {
        Reader *mesh_reader;
        parse_dataset_path(mesh_path, mesh_loc, mesh_file, mesh_ext);
        new_reader(&mesh_reader, mesh_ext);
        mesh_reader->open(mesh_file);
        if (coords == 0)
        {
            if (mesh_reader->getType() == Reader::HDF_READER)
            {
                coords_loc = mesh_loc + "/coordinates";
            }
            mesh_reader->get_coordinates(coords_loc.c_str(), &coords, &nno, &nsd);
        }
        if (connect == 0)
        {
            if (mesh_reader->getType() == Reader::HDF_READER)
            {
                connect_loc = mesh_loc + "/connectivity";
            }
            mesh_reader->get_connectivity(connect_loc.c_str(), &connect, &nel, &ndofs);
        }
        //mesh_reader->close();
    }

    if ((coords != 0) && (connect != 0))
    {
        meshPart = new MeshPart();

        meshPart->nno = nno;
        meshPart->nsd = nsd;
        meshPart->coords = coords;

        meshPart->nel = nel;
        meshPart->ndofs = ndofs;
        meshPart->connect = connect;
    }
    else
    {
        if (coords == 0)
        {
            cerr << "MeshIO::load() error: Could not find mesh coordinates";
            cerr << endl;
        }
        if (connect == 0)
        {
            cerr << "MeshIO::load() error: Could not find mesh connectivity";
            cerr << endl;
        }
    }

}

// ---------------------------------------------------------------------------
