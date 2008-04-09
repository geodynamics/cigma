#include <iostream>
#include <cstdlib>
#include <cassert>
#include "FieldSet.h"
#include "ZeroField.h"
#include "GaleBenchmarkFields.h"
#include "OkadaBenchmarkFields.h"
#include "TestBenchmarkFields.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

//
// This method will initialize our FieldSet container with a
// number of predefined fields. Others fields may be added
//
void FieldSet::initialize()
{
    //
    // XXX: In retrospect, we need to modify UserField to implement
    // lazy loading (do not initialize in constructor).
    //
    ZeroField *zero = new ZeroField();
    addField("zero", zero);

    typedef okada::strike_slip::Disloc3d OkadaStrikeSlip;
    OkadaStrikeSlip *disloc3d = new OkadaStrikeSlip();
    addField("disloc3d", disloc3d);

    typedef gale::circular_inclusion::Pressure GCIPressure;
    GCIPressure *pressure = new GCIPressure();
    addField("gale.circular_inclusion.pressure", pressure);

    typedef cigma::test_benchmarks::Cube CubeBM;
    CubeBM *cube = new CubeBM();
    addField("test.cube", cube);

    typedef cigma::test_benchmarks::Square SquareBM;
    SquareBM *square = new SquareBM();
    addField("test.square", square);
}


// ---------------------------------------------------------------------------

FieldSet::FieldSet()
{
}

FieldSet::~FieldSet()
{
    FieldMap::iterator it;
    for (it = fields.begin(); it != fields.end(); ++it)
    {
        Field *field = it->second;
        if (field != 0)
        {
            delete field;
        }
    }
}


// ---------------------------------------------------------------------------

void FieldSet::addField(std::string name, Field *field)
{
    assert(field != 0);
    fields[name] = field;
}

Field *FieldSet::getField(std::string name)
{
    Field *field = 0;
    FieldMap::iterator it = fields.find(name);
    if (it != fields.end())
    {
        field = it->second;
    }
    return field;
}


// ---------------------------------------------------------------------------
