#include <cassert>
#include "Command.h"

cigma::Command::~Command()
{
}

bool cigma::Command::helpFlag(AnyOption *opt)
{
    if (opt != 0)
    {
        return opt->getFlag("help") || opt->getFlag('h');
    }
    return false;
}
