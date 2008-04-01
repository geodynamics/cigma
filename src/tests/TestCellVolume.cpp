#include <iostream>
#include <cassert>
#include "../Tet.h"
#include "../Hex.h"
#include "../Tri.h"
#include "../Quad.h"

using namespace std;
using namespace cigma;

int main()
{
    Tri tri;
    Quad quad;
    Tet tet;
    Hex hex;
    cout << "tet.volume() = " << tet.volume() << endl;
    cout << "hex.volume() = " << hex.volume() << endl;
    cout << "tri.volume() = " << tri.volume() << endl;
    cout << "quad.volume() = " << quad.volume() << endl;
    return 0;
}
