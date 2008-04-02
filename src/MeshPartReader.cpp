#include <iostream>
#include <cstdlib>
#include <cassert>
#include "MeshPartReader.h"
#include "StringUtils.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

MeshPartReader::MeshPartReader()
{
    meshPart = 0;
    meshReader = 0;
    coordsReader = 0;
    connectReader = 0;
}

MeshPartReader::~MeshPartReader()
{
    if (meshPart != 0)
    {
        if (meshPart->coords != 0)
        {
            delete [] meshPart->coords;
            meshPart->coords = 0;
        }

        if (meshPart->connect != 0)
        {
            delete [] meshPart->connect;
            meshPart->connect = 0;
        }

        delete meshPart;
        meshPart = 0;
    }
}

// ---------------------------------------------------------------------------

void MeshPartReader::load_args(AnyOption *opt, const char *opt_prefix)
{
    assert(opt != 0);

    char *in;
    string optstr;
    string mesh_name = opt_prefix;

    in = opt->getValue(mesh_name.c_str());
    if (in != 0)
    {
        this->meshPath = in;
    }

    optstr = mesh_name + "-coordinates";
    in = opt->getValue(optstr.c_str());
    if (in != 0)
    {
        this->coordsPath = in;
    }

    optstr = mesh_name + "-connectivity";
    in = opt->getValue(optstr.c_str());
    if (in != 0)
    {
        this->connectPath = in;
    }
}

void MeshPartReader::validate_args(const char *cmd_name)
{
    if (has_paths())
    {
        if (meshPath == "")
        {
            if (coordsPath == "")
            {
                cerr << cmd_name
                     << ": Missing mesh coordinates option"
                     << endl;
                exit(1);
            }
            if (connectPath == "")
            {
                cerr << cmd_name
                     << ": Missing mesh connectivity option"
                     << endl;
                exit(1);
            }
        }
    }
}

// ---------------------------------------------------------------------------

bool MeshPartReader::has_paths()
{
    return (meshPath != "") || (coordsPath != "") || (connectPath != "");
}

bool MeshPartReader::has_coords_path()
{
    return (meshPath != "") || (coordsPath != "");
}

bool MeshPartReader::has_connect_path()
{
    return (meshPath != "") || (connectPath != "");
}

bool MeshPartReader::has_valid_paths()
{
    // empty state is valid
    if (!has_paths())
    {
        return true;
    }

    // require both coords and connect paths if mesh path is absent
    return (meshPath != "") || ((coordsPath != "") && (connectPath != ""));
}

bool MeshPartReader::prepare_paths()
{
    // This function initializes the required variables used in load_mesh()
    //
    //  precondition: all path variables need to be set
    //                appropriately by calling load_args()
    //
    //  postcondition: if successful, location & file variables
    //                 will be ready for use in load_mesh()

    if (!(has_coords_path() && has_connect_path()))
    {
        // not enough information available to set location variables.
        // top level program should use validate_args() to detect this
        // condition ahead of time and bail out
        return false;
    }

    // 
    // Note that each of coordsPath and connectPath will override
    // the corresponding component in meshPath
    //
    if (coordsPath != "")
    {
        parse_dataset_path(coordsPath, coordsLoc, coordsFile, coordsExt);
    }
    if (connectPath != "")
    {
        parse_dataset_path(connectPath, connectLoc, connectFile, connectExt);
    }
    if (meshPath != "")
    {
        parse_dataset_path(meshPath, meshLoc, meshFile, meshExt);

        if (coordsPath == "")
        {
            // no coords path was specified...
            // read coordinates from mesh file
            // coordsLoc will have to be determine later in load_mesh()
            coordsLoc  = "";
            coordsFile = meshFile;
            coordsExt  = meshExt;
        }
        else
        {
            if (coordsLoc == "")
            {
                // reinterpret the meaning of --mesh-coordinates
                coordsLoc  = coordsFile;
                coordsFile = meshFile;
                coordsExt  = meshExt;
            }
        }

        if (connectPath == "")
        {
            // no connect path specified...
            // read connectivity from mesh file
            // connectLoc will have to be determined later in load_mesh()
            connectLoc  = "";
            connectFile = meshFile;
            connectExt  = meshExt;
        }
        else
        {
            // reintepret the meaning of --mesh-connectivity
            connectLoc  = connectFile;
            connectFile = meshFile;
            connectExt  = meshExt;
        }
    }

    return true;
}

// ---------------------------------------------------------------------------

void MeshPartReader::load_mesh()
{
    //
    // This function initializes the meshPart member variable.
    // Failure can be detected by checking whether meshPart is 0.
    //

    // Nothing to do if paths are empty
    if (!has_paths())
    {
        return;
    }

    bool ready = prepare_paths();
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

    //
    // Instantiate readers
    //
    if (meshFile != "")
    {
        meshReader = NewReader(meshExt.c_str());
    }
    if (coordsFile != "")
    {
        if (coordsFile == meshFile)
        {
            coordsReader = meshReader;
        }
        else
        {
            coordsReader = NewReader(coordsExt.c_str());
        }
    }
    if (connectFile != "")
    {
        if (connectFile == meshFile)
        {
            connectReader = meshReader;
        }
        else
        {
            connectReader = NewReader(connectExt.c_str());
        }
    }
    if (coordsReader == 0)
    {
        cerr << "Error: Could not make reader for mesh coordinates" << endl;
        exit(1);
    }
    if (connectReader == 0)
    {
        cerr << "Error: Could not make reader for mesh connectivity" << endl;
        exit(1);
    }

    //
    // Open files
    //
    if (meshReader != 0)
    {
        ierr = meshReader->open(meshFile.c_str());
        if (ierr < 0)
        {
            cerr << "Error: Could not open mesh file " << meshFile << endl;
            exit(1);
        }
    }
    if (coordsReader != meshReader)
    {
        ierr = coordsReader->open(coordsFile.c_str());
        if (ierr < 0)
        {
            cerr << "Error: Could not open mesh-coordinates file " << coordsFile << endl;
            exit(1);
        }
    }
    if (connectReader != meshReader)
    {
        ierr = connectReader->open(connectFile.c_str());
        if (ierr < 0)
        {
            cerr << "Error: Could not open mesh-connectivity file " << connectFile << endl;
            exit(1);
        }
    }

    //
    // Last chance to determine coordsLoc and connectLoc.
    // For HDF5 files, check dataset or attribute first?
    // Also, if meshLoc is empty, does it *have* to be "/"?
    //
    if (coordsLoc == "")
    {
        if (coordsReader->getType() == Reader::HDF_READER)
        {
            coordsLoc = meshLoc + "/coordinates";
            // If at this point, the coordsLoc dataset doesn't exist,
            // then determine the following before bailing out:
            //  * assert that meshLoc is a group
            //  * the group meshLoc has a Coordinates string attribute
            //  * load the Coordinates attribute into coordsLoc variable
        }
    }
    if (connectLoc == "")
    {
        if (connectReader->getType() == Reader::HDF_READER)
        {
            connectLoc = meshLoc + "/connectivity";
            // If at this point, the connectLoc dataset doesn't exist,
            // determine the following before bailing out:
            //  * assert that meshLoc is a group 
            //  * the group meshLoc has a Connectivity string attribute
            //  * load the Connectivity string attribute into connectLoc
        }
    }

    //
    // Read datasets
    //
    ierr = coordsReader->get_coordinates(coordsLoc.c_str(), &coords, &nno, &nsd);
    if (ierr < 0)
    {
        cerr << "Error: Could not read mesh coordinates!" << endl;
        exit(2);
    }
    assert(coords != 0);

    ierr = connectReader->get_connectivity(connectLoc.c_str(), &connect, &nel, &ndofs);
    if (ierr < 0)
    {
        cerr << "Error: Could not read mesh connectivity!" << endl;
        exit(2);
    }
    assert(connect != 0);


    if ((coords != 0) && (connect != 0))
    {
        meshPart = new MeshPart();

        meshPart->nno = nno;
        meshPart->nsd = nsd;
        meshPart->coords = coords;

        meshPart->nel = nel;
        meshPart->ndofs = ndofs;
        meshPart->connect = connect;

        // XXX: can we decide at this point which cell to assign?
        meshPart->set_cell();
    }

    //
    // Clean up
    //
    if (meshReader != 0)
    {
        if (coordsReader == meshReader)
        {
            coordsReader = 0;
        }
        if (connectReader == meshReader)
        {
            connectReader = 0;
        }
        delete meshReader;
        meshReader = 0;
    }
    if (coordsReader != 0)
    {
        delete coordsReader;
        coordsReader = 0;
    }
    if (connectReader != 0)
    {
        delete connectReader;
        connectReader = 0;
    }
}

// ---------------------------------------------------------------------------
