#include <iostream>
#include <cassert>
#include "ListCmd.h"
#include "VtkList.h"

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
    cout << "Calling cigma::ListCmd::setupOptions()" << endl;

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
    cout << "Calling cigma::ListCmd::configure()" << endl;

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
    cout << "Calling cigma::ListCmd::run()" << endl;

    if (filename == "")
    {
        return 1;
    }

    vtkls(filename.c_str());

    return 0;
}


// ---------------------------------------------------------------------------
