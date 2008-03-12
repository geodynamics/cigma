#include <iostream>
#include "../ZeroFunction.h"

using namespace std;
using namespace cigma;

int main()
{
    ZeroFunction *F = new ZeroFunction();

    const int n = 10;
    double x[n], y[n];

    int i;
    for (i = 0; i < n; i++)
    {
        x[i] = i / (1.0 * n);
    }

    F->set_shape(1,1);

    for (i = 0; i < n; i++)
    {
        F->eval(&x[i], &y[i]);
    }

    for (i = 0; i < n; i++)
    {
        cout << "F(" << x[i] << ") = " << y[i] << endl;
    }

    return 0;
}
