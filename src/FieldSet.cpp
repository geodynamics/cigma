#include <iostream>
#include <cstdlib>
#include <cassert>
#include "FieldSet.h"
#include "ZeroField.h"
#include "GaleBenchmarkFields.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

//
// This method will initialize our FieldSet container with a
// number of predefined fields. Others may be added
//
void FieldSet::initialize()
{

    ZeroField *zero = new ZeroField();
    addField("zero", zero);

    typedef gale::circular_inclusion::Pressure GCIPressure;
    GCIPressure *pressure = new GCIPressure();
    addField("gale.circular_inclusion.pressure", pressure);
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
