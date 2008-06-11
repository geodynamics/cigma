#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string>

#include "ListCmd.h"
#include "VtkList.h"
#include "StringUtils.h"
#include "PathUtils.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

ListCmd::ListCmd()
{
    name = "list";
}

ListCmd::~ListCmd()
{
}

// ---------------------------------------------------------------------------


void ListCmd::setupOptions(AnyOption *opt)
{
    //cout << "Calling cigma::ListCmd::setupOptions()" << endl;

    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("    cigma list <file>");
    opt->addUsage("");
    opt->addUsage("Supported file extensions:");
    opt->addUsage("    .h5                - Cigma HDF5 files");
    opt->addUsage("    .vtk               - Legacy VTK files");
    opt->addUsage("    .vtu, .vts . vtr   - XML VTK files");
    opt->addUsage("    .pvtu .pvts .pvtr  - XML Parallel VTK files");
    

    /* setup flags and options */
    opt->setFlag("help", 'h');

    return;
}


void ListCmd::configure(AnyOption *opt)
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

    if (filename == "")
    {
        opt->printUsage();
    }
    return;
}


int ListCmd::run()
{
    //cout << "Calling cigma::ListCmd::run()" << endl;

    if (filename == "")
    {
        return 1;
    }

    string fileroot;
    string extension;

    path_splitext(filename, fileroot, extension);
    
    if (HdfExtension(extension.c_str()))
    {
        int ret;
        string cmd = "h5ls -r ";
        cmd += filename;
        ret = system(cmd.c_str());
        if (ret < 0)
        {
            cerr << "list: Could not find h5ls in your PATH" << endl;
        }
        return ret;
    }
    else if (VtkExtension(extension.c_str()))
    {
        vtk_list(filename.c_str());
    }
    else
    {
        cerr << "list: Unsupported file extension "
             << " (found '" << extension << "')" << endl;
        return 2;
    }

    return 0;
}


// ---------------------------------------------------------------------------
