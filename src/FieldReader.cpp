#include <iostream>
#include <cstdlib>
#include <cassert>
#include "FieldReader.h"


using namespace std;
using namespace cigma;


// ---------------------------------------------------------------------------

FieldReader::FieldReader()
{
    field = 0;
}

FieldReader::~FieldReader()
{
    if (field != 0)
    {
        // XXX: traverse field and delete everything
    }
}

// ---------------------------------------------------------------------------

void FieldReader::load_args(AnyOption *opt, const char *opt_prefix)
{

}


void FieldReader::validate_args(const char *cmd_name)
{

}


// ---------------------------------------------------------------------------

void FieldReader::load_field()
{
    int dofs_nno, dofs_valdim;
    double *dofs;


}

// ---------------------------------------------------------------------------
