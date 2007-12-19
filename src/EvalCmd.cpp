#include <iostream>
#include <cassert>
#include "EvalCmd.h"

// ---------------------------------------------------------------------------

cigma::EvalCmd::EvalCmd()
{
    name = "eval";
}

cigma::EvalCmd::~EvalCmd()
{
}

// ---------------------------------------------------------------------------

void cigma::EvalCmd::setupOptions(AnyOption *opt)
{
    std::cout << "Calling cigma::EvalCmd::setupOptions()" << std::endl;
    assert(opt != 0);

    /* setup usage */
    opt->addUsage("Usage:");
    opt->addUsage("");
    opt->addUsage("   cigma eval [args ...]");

    /* setup flags and options */
    opt->setFlag("help", 'h');

    return;
}

void cigma::EvalCmd::configure(AnyOption *opt)
{
    std::cout << "Calling cigma::EvalCmd::configure()" << std::endl;
    return;
}

int cigma::EvalCmd::run()
{
    std::cout << "Calling cigma::EvalCmd::run()" << std::endl;
    return 0;
}
