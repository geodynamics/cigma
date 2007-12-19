#include <iostream>
#include <cstdlib>
#include "CommandSet.h"

using namespace cigma;

int main(int argc, char *argv[])
{
    CommandSet commands;
    commands.initialize();
    return commands.main(argc, argv);
}

