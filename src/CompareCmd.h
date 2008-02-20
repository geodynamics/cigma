#ifndef __COMPARE_CMD_H__
#define __COMPARE_CMD_H__

#include <string>
#include "Command.h"
#include "MeshPart.h"
#include "Field.h"
#include "FE_Field.h"
#include "ResidualField.h"
#include "Writer.h"
#include "Reader.h"
#include "MeshIO.h"
#include "QuadratureIO.h"
#include "FieldIO.h"

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
    ResidualField *residuals;

public:
    MeshIO meshIO;
    QuadratureIO quadratureIO;
    FieldIO firstIO;
    FieldIO secondIO;
    std::string output_path;

public:
    bool verbose;
    int output_frequency;
};


#endif
