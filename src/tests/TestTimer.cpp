#include <iostream>
#include <string>
#include "../Timer.h"
#include "../StringUtils.h"

using namespace std;

int main(int argc, char *argv[])
{
    int nel = 1000000;
    int freq = 100;

    bool verbose = true;

    
    /* Convert argv[1] to string */
    string f = "100";
    if (argc > 1)
    {
        f = argv[1];
    }

    string_to_int(f,freq);

    if (freq <= 0)
    {
        cerr << "Please specify a positive integer "
             << "(got '" << f << "')" << endl;
        return 1;
    }

    cout << "Timing empty loop:" << endl;
    cout << "  output frequency = " << freq << endl;
    cout << "  number of iterations = " << nel << endl;
    cout << endl;

    /* Time an empty loop */
    Timer timer;
    timer.print_header(cout, "e");
    timer.start(nel);
    timer.update(0);
    for (int e = 0; e < nel; e++)
    {
        if (verbose && ((e+1) % freq == 0))
        {
            timer.update(e+1);
            cout << timer;
        }
    }
    timer.update(nel);
    cout << timer << endl;

    return 0;
}
