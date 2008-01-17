#ifndef __COMPARE_CMD_H__
#define __COMPARE_CMD_H__

#include "Command.h"
#include "MeshPart.h"
#include "Field.h"
#include "FE_Field.h"
#include "Writer.h"
#include "VtkUgReader.h"
//#include "HdfReader.h"

namespace cigma
{
    class CompareCmd;
}


/*
 * Callback object for `cigma compare [args ...]'
 */
class cigma::CompareCmd : public Command
{
public:
    CompareCmd();
    ~CompareCmd();

public:
    void setupOptions(AnyOption *opt);
    void configure(AnyOption *opt);
    int run();

public:
    Quadrature *quadrature;
    MeshPart *mesh;
    FE_Field *field_a;
    FE_Field *field_b;
    Field *residuals;
    std::string output_filename, output_name;
    std::string locationA, locationB;
    VtkUgReader readerA, readerB;
    //HdfReader h5_readerA, h5_readerB;
};


#endif
