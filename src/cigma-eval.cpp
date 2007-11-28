#include <iostream>
#include "anyoption.h"

using namespace std;

void print_usage(void)
{
    cerr << "Usage: cigma-compare --field=field.h5:/model/variables/displacement/step0" << endl
         << "                     --points=points.h5" << endl
         << "                     --output=values.h5" << endl;
}

int main(int argc, char **argv)
{
    AnyOption *opt = new AnyOption();

    opt->setFlag("help", 'h');
    opt->setOption("field",'f');
    opt->setOption("points",'p');
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

    if (opt->getValue("field") != NULL)
    {
        cout << "field = \"" << opt->getValue("first") << "\"" << endl;
    }
    else
    {
        cout << "Error: Please specify a field with --field" << endl;
        delete opt;
        return -1;
    }


    if (opt->getValue("points") != NULL)
    {
        cout << "points = \"" << opt->getValue("points") << "\"" << endl;
    }
    else
    {
        cout << "Error: Please specify points with --points" << endl;
        delete opt;
        return -2;
    }

    if (opt->getValue("output") != NULL)
    {
        cout << "output = \"" << opt->getValue("output") << "\"" << endl;
    }
    else
    {
        cout << "Error: Please specify output file with --output" << endl;
        delete opt;
        return -3;
    }

    delete opt;
    return 0;
}
