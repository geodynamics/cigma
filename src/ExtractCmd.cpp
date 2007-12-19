#include <iostream>
#include <cassert>
#include "ExtractCmd.h"

// ---------------------------------------------------------------------------

cigma::ExtractCmd::ExtractCmd()
{
    name = "extract";
}

cigma::ExtractCmd::~ExtractCmd()
{
}

// ---------------------------------------------------------------------------

void cigma::ExtractCmd::setupOptions(AnyOption *opt)
{
    std::cout << "Calling cigma::ExtractCmd::setupOptions()" << std::endl;

    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("");
    opt->addUsage("   cigma extract [args ...]");
    opt->addUsage("");
    opt->addUsage("");
    opt->addUsage("");

    /* setup flags and options */
    opt->setFlag("help", 'h');
    opt->setOption("order", 'o');
    opt->setOption("meshfile");
    opt->setOption("meshpath");
}

void cigma::ExtractCmd::configure(AnyOption *opt)
{
    std::cout << "Calling cigma::ExtractCmd::configure()" << std::endl;
    return;
}

int cigma::ExtractCmd::run()
{
    std::cout << "Calling cigma::ExtractCmd::run()" << std::endl;
    return 0;
}
