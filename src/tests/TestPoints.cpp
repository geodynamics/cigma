#include <iostream>
#include <iomanip>
#include "../Points.h"

using namespace std;
using namespace cigma;

int main()
{

    double coords[4*3] = {
        -1, -1, -1,
        +1, -1, -1,
        -1, +1, -1,
        -1, -1, +1
    };

    Points points;

    points.set_data(coords, 4, 3);

    cout << std::showpos
         << std::fixed
         << std::setprecision(1);

    int i,j;
    for (i = 0; i < points.n_points(); i++)
    {
        for (j = 0; j < points.n_dim(); j++)
        {
            cout << points(i,j) << " ";
        }
        cout << endl;
    }

    return 0;
}
