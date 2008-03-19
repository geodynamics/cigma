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

MeshIO::MeshIO()
{
    meshPart = 0;
    reader = coords_reader = connect_reader = 0;
    writer = coords_writer = connect_writer = 0;
}

MeshIO::~MeshIO()
{
    if (meshPart != 0)
    {
        // XXX: traverse meshPart structure and delete everything
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

bool MeshIO::prepare()
{
    // This function initializes the required variables
    // for use in MeshIO::load()
    //
    //  precondition: all path variables need to be set
    //                appropriately by calling load_args()
    //
    //  postcondition: if successful, location & file variables
    //                 will be ready for use in load()
    //

    if (!(has_coords_path() && has_connect_path()))
    {
        // not enough information available to set location variables.
        // top level program should use validate_args() to detect this
        // condition ahead of time and bail out
        return false;
    }

    //
    // Note that each of coords_path and connect_path will override
    // the corresponding component in mesh_path.
    //
    if (coords_path != "")
    {
        parse_dataset_path(coords_path, coords_loc, coords_file, coords_ext);
    }
    if (connect_path != "")
    {
        parse_dataset_path(connect_path, connect_loc, connect_file, connect_ext);
    }
    if (mesh_path != "")
    {
        parse_dataset_path(mesh_path, mesh_loc, mesh_file, mesh_ext);

        if (coords_path == "")
        {
            // no coords path specified...
            // read coords from mesh file
            // coords_loc will have to be determined later in load()
            coords_loc  = "";
            coords_file = mesh_file;
            coords_ext  = mesh_ext;
        }
        else
        {
            if (coords_loc == "")
            {
                // reinterpret the meaning of --mesh-coordinates
                coords_loc  = coords_file;
                coords_file = mesh_file;
                coords_ext  = mesh_ext;
            }
        }

        if (connect_path == "")
        {
            // no connect path specified...
            // read connectivity from mesh file
            // connect_loc will have to be determined later in load()
            connect_loc  = "";
            connect_file = mesh_file;
            connect_ext  = mesh_ext;
        }
        else
        {
            if (connect_loc == "")
            {
                // reinterpret the meaning of --mesh-connectivity
                connect_loc  = connect_file;
                connect_file = mesh_file;
                connect_ext  = mesh_ext;
            }
        }
    }

    return true;
}

void MeshIO::load()
{
    // 
    // The sole purpose of this function is to initialize
    // the meshPart member variable. Thus, failure can be detected
    // by checking whether meshPart is 0.
    //


    //cout << "Calling MeshIO::load()" << endl;


    // Skip this method if paths are empty.
    // This behavior is necessary because the --mesh option
    // is not required on the command line
    if (!has_paths())
    {
        return;
    }

    // bail out if prepare() fails
    bool ready = prepare();
    if (!ready)
    {
        return;
    }


    //
    // Local declarations
    //
    int ierr;

    int nno, nsd;
    double *coords = 0;

    int nel, ndofs;
    int *connect = 0;

    nno = nsd = 0;
    nel = ndofs = 0;


    // To implement at this point:
    //  XXX: check whether files exist if we are reading
    //  XXX: emit warnings or bail out if we are writing an existing file?
    //  XXX: check file's magic number and overwrite the extension if it doesn't match
    //


    // 
    // Instantiate readers
    //
    if (mesh_file != "")
    {
        new_reader(&reader, mesh_ext);
    }
    if (coords_file != "")
    {
        if (coords_file == mesh_file)
        {
            coords_reader = reader;
        }
        else
        {
            new_reader(&coords_reader, coords_ext);
        }
    }
    if (connect_file != "")
    {
        if (connect_file == mesh_file)
        {
            connect_reader = reader;
        }
        else
        {
            new_reader(&connect_reader, connect_ext);
        }
    }
    assert(coords_reader != 0);
    assert(connect_reader != 0);


    //
    // Open files
    //
    if (reader != 0)
    {
        ierr = reader->open(mesh_file);
        assert(ierr == 0);
    }
    if (coords_reader != reader)
    {
        ierr = coords_reader->open(coords_file);
        assert(ierr == 0);
    }
    if (connect_reader != reader)
    {
        ierr = connect_reader->open(connect_file);
        assert(ierr == 0);
    }


    //
    // Last chance to determine coords_loc and connect_loc
    //  XXX: for HDF5 files, check dataset or attribute first?
    //  XXX: for HDF5 files, if mesh_loc is empty, does it *have* to be "/"?
    //
    if (coords_loc == "")
    {
        if (coords_reader->getType() == Reader::HDF_READER)
        {
            coords_loc = mesh_loc + "/coordinates";
            // XXX: if at this point the coords_loc dataset doesn't exist,
            // determine the following before bailing out:
            //  * assert that mesh_loc is a group
            //  * the group mesh_loc has a CoordLocation string attribute
            //  * load the CoordLocation string attribute into coords_loc
        }
        else if (coords_reader->getType() == Reader::TXT_READER)
        {
            // XXX: not yet implemented!
            // XXX: bail out with a better message
            // Here we would need support for reading multiple datasets
            // from a single text file (low priority since we can use
            // HDF5 for this very purpose).
            assert(false);
        }
        else if (coords_reader->getType() == Reader::VTK_READER)
        {
            // XXX: it is not necessary to set coords_loc for VTK files
            // since VtkReader::get_coordinates() ignores its location argument
        }

    }
    if (connect_loc == "")
    {
        if (connect_reader->getType() == Reader::HDF_READER)
        {
            connect_loc = mesh_loc + "/connectivity";
            // XXX: if at this point the connect_loc dataset doesn't exist,
            // determine the following before bailing out:
            //  * assert that mesh_loc is a group
            //  * the group mesh_loc has a ConnectLocation string attribute
            //  * load the ConnectLocation string attribute into coords_loc
        }
    }

    //
    // Read datasets
    //  XXX: change these functions to return an error code
    //  XXX: need to check return values for failure
    coords_reader->get_coordinates(coords_loc.c_str(), &coords, &nno, &nsd);
    connect_reader->get_connectivity(connect_loc.c_str(), &connect, &nel, &ndofs);

    /*
    cout << "*** "
         << "nno = " << nno << ", "
         << "nsd = " << nsd << ", "
         << "nel = " << nel << ", "
         << "ndofs = " << ndofs << endl;
    // */



    /* XXX: use auto_ptr for the local readers, so we can throw exceptions
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
    } // */




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
            cerr << "MeshIO::load() error: "
                 << "Could not find mesh coordinates"
                 << endl;
        }
        if (connect == 0)
        {
            cerr << "MeshIO::load() error: "
                 << "Could not find mesh connectivity"
                 << endl;
        }
    }

}

// ---------------------------------------------------------------------------
