#include <iostream>
#include <cassert>
#include "SkelCmd.h"


// ---------------------------------------------------------------------------

cigma::SkelCmd::SkelCmd()
{
    name = "skel"; // XXX: change name of your command here
}


cigma::SkelCmd::~SkelCmd()
{
}


// ---------------------------------------------------------------------------

void cigma::SkelCmd::setupOptions(AnyOption *opt)
{
    /*
     * Here, we define the command line options and flags that will be read
     * from the cmdline params (argc,argv). We also define the usage in
     * here (triggered by `cigma help skel').
     */
    std::cout << "Calling cigma::SkelCmd::setupOptions()" << std::endl;
    assert(opt != 0);

    /* setup usage */

    opt->addUsage("Usage:");
    opt->addUsage("");
    opt->addUsage("   cigma skel [args]");
    opt->addUsage("");
    opt->addUsage("");


    /* setup flags and options */

    opt->setFlag("help",'h'); // XXX: need to set at least one flag or option (bug in AnyOption)
}


void cigma::SkelCmd::configure(AnyOption *opt)
{
    /*
     * Here, we perform validation on the flag and option values, and
     * correspondingly change the internal state of our object
     */
    std::cout << "Calling cigma::SkelCmd::configure()" << std::endl;
    assert(opt != 0);
    myVar1 = 1;
    myVar2 = 2.0;
}


int cigma::SkelCmd::run()
{
    /*
     * Here we perform the action that represents our command,
     * and then return an appropriate exit code.
     */
    std::cout << "Calling cigma::SkelCmd::run()" << std::endl;

    myMethod1();
    myMethod2();

    return 0;
}


// ---------------------------------------------------------------------------

void cigma::SkelCmd::myMethod1()
{
    myVar1 += 1;
}

void cigma::SkelCmd::myMethod2()
{
    myVar2 += 2.0;
}

// ---------------------------------------------------------------------------
