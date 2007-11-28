#include <iostream>
#include "anyoption.h"

using namespace std;


void print_usage(void)
{
    cerr << "Usage: cigma-extract --rule=qr.h5:/FIAT/cools/tet_4" << endl
         << "                     --mesh=meshes.h5:/model/mesh" << endl
         << "                     --output=points.h5" << endl;
}



int main(int argc, char **argv)
{
    AnyOption *opt = new AnyOption();

    opt->setFlag("help", 'h');
    opt->setOption("rule", 'r');
    opt->setOption("mesh", 'm');
    opt->setOption("output", 'o');

    opt->processCommandArgs(argc, argv);

    if (!opt->hasOptions())
    {
        print_usage();
        delete opt;
        return 0;
    }

    if (opt->getFlag("help") || opt->getFlag('h'))
    {
        print_usage();
        delete opt;
        return 0;
    }

    if (opt->getValue("rule") != NULL)
    {
        cout << "rule = \"" << opt->getValue("rule") << "\"" << endl;
    }
    else
    {
        cout << "Error: Please specify a quadrature rule with --rule" << endl;
        delete opt;
        return -1;
    }


    if (opt->getValue("mesh") != NULL)
    {
        cout << "mesh = \"" << opt->getValue("mesh") << "\"" << endl;
    }
    else
    {
        cout << "Error: Please specify an input mesh with --mesh" << endl;
        delete opt;
        return -2;
    }

    if (opt->getValue("output") != NULL)
    {
        cout << "output = \"" << opt->getValue("output") << "\"" << endl;
    }
    else
    {
        cout << "Error: Please specify an output file with --output" << endl;
        delete opt;
        return -3;
    }


    delete opt;
    return 0;
}


