#include <iostream>
#include <cassert>
#include "../VtkReader.h"

using namespace std;
using namespace cigma;


int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        cerr << "Usage: " << argv[0] << " vtk-file" << endl;
        return -1;
    }

    string filename = argv[1];
    cout << "Reading file '" << filename << "'\n";

    VtkReader reader;

    reader.open(filename.c_str());
    
    return 0;    
}
