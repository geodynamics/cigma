#include <iostream>
#include <cstdlib>
#include <cassert>
#include "../TextWriter.h"
#include "../HdfReader.h"

using namespace cigma;

int main(void)
{
    TextWriter *writer = new TextWriter();
    writer->fp = stdout;

    {

    }

    writer->fp = NULL;
    delete writer;
}
