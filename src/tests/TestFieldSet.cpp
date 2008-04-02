#include <iostream>
#include <limits>
#include <cassert>
#include "../ZeroField.h"
#include "../FieldSet.h"

using namespace std;
using namespace cigma;

int main()
{
    Field *field;
    double x[3];
    double y[3];

    FieldSet fieldset;
    fieldset.initialize();
    
    /* try a known field */
    field = fieldset.getField("zero");
    ZeroField *zero = static_cast<ZeroField*>(field);
    zero->set_shape(3,3);
    x[0] = x[1] = x[2] = 0;
    y[0] = y[1] = y[2] = numeric_limits<double>::quiet_NaN();
    field->eval(x,y);
    cout << "zero"
         << "(" << x[0] << "," << x[1] << "," << x[2] << ") = "
         << "(" << y[0] << "," << y[1] << "," << y[2] << ")"
         << endl;

    /* try gale's circular_inclusion benchmark */
    field = fieldset.getField("gale.circular_inclusion.pressure");
    x[0] = 0.5;
    x[1] = 0.57;
    y[0] = numeric_limits<double>::infinity();
    field->eval(x,y);
    cout << "pressure"
         << "(" << x[0] << "," << x[1] << ") = "
         << y[0]
         << endl;

    /* test a non-existent field */
    field = fieldset.getField("foo");
    assert(field == 0);


    return 0;
}
