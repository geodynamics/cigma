#include <iostream>
#include <iomanip>
#include "../Points.h"

int main()
{
    using namespace std;
    using namespace cigma;

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

    for (int i = 0; i < points.n_points(); i++)
    {
        for (int j = 0; j < points.n_dim(); j++)
        {
            cout << points(i,j) << " ";
        }
        cout << endl;
    }

    return 0;
}
