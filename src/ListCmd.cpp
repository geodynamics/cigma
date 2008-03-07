#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string>
#include "ListCmd.h"
#include "VtkList.h"
#include "StringUtils.h"

using namespace std;

// ---------------------------------------------------------------------------

cigma::ListCmd::ListCmd()
{
    name = "list";
}

cigma::ListCmd::~ListCmd()
{
}

// ---------------------------------------------------------------------------


void cigma::ListCmd::setupOptions(AnyOption *opt)
{
    //cout << "Calling cigma::ListCmd::setupOptions()" << endl;

    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("   cigma list FILE");

    /* setup flags and options */
    opt->setFlag("help", 'h');

    return;
}


void cigma::ListCmd::configure(AnyOption *opt)
{
    //cout << "Calling cigma::ListCmd::configure()" << endl;

    assert(opt != 0);
    char *in = 0;

    int argc = opt->getArgc();
    if (argc > 0)
    {
        in = opt->getArgv(0);
        filename = in;
    }

    return;
}


int cigma::ListCmd::run()
{
    //cout << "Calling cigma::ListCmd::run()" << endl;

    if (filename == "")
    {
        return 1;
    }

    string fileroot;
    string extension;

    path_splitext(filename, fileroot, extension);
    
    if (extension == ".vtk")
    {
        vtkls(filename.c_str());
    }
    else if (extension == ".h5")
    {
        int ret;
        string cmd = "h5ls -r ";
        cmd += filename;
        ret = system(cmd.c_str());
        return ret;
    }
    else
    {
        cerr << "list: File extension must be .h5 or .vtk" << endl;
        return 2;
    }

    return 0;
}


// ---------------------------------------------------------------------------
