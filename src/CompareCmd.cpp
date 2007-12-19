#include <iostream>
#include <cassert>
#include "CompareCmd.h"

// ---------------------------------------------------------------------------

cigma::CompareCmd::CompareCmd()
{
    name = "compare";
}

cigma::CompareCmd::~CompareCmd()
{
}

// ---------------------------------------------------------------------------

void cigma::CompareCmd::setupOptions(AnyOption *opt)
{
    std::cout << "Calling cigma::CompareCmd::setupOptions()" << std::endl;

    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("");
    opt->addUsage("   cigma compare [args ...]");

    /* setup flags and options */
    opt->setFlag("help", 'h');
    opt->setOption("FileA");
    opt->setOption("PathA");
    opt->setOption("FileB");
    opt->setOption("PathB");
}

void cigma::CompareCmd::configure(AnyOption *opt)
{
    std::cout << "Calling cigma::CompareCmd::configure()" << std::endl;
    return;
}

int cigma::CompareCmd::run()
{
    std::cout << "Calling cigma::CompareCmd::run()" << std::endl;
    return 0;
}
