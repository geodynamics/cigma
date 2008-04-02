#ifndef __COMPARE_CMD_H__
#define __COMPARE_CMD_H__

#include <string>
#include "Command.h"
#include "Field.h"
#include "FieldSet.h"
#include "FieldReader.h"
#include "MeshPart.h"
#include "QuadratureRule.h"
#include "QuadratureReader.h"
#include "Residuals.h"
#include "Timer.h"


namespace cigma
{
    class CompareCmd;
}

/**
 * @brief Callback object for `cigma compare [options]'
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
    void start_timer(int nel, const char *firstcol);
    void update_timer(int e);
    void end_timer();

public:
    Field *field_a;
    Field *field_b;
    MeshPart *meshPart;
    QuadratureRule *quadrature;
    Residuals *residuals;

public:
    FieldSet fieldSet;
    FieldReader firstReader;
    FieldReader secondReader;
    QuadratureReader quadratureReader;
    std::string outputFile;

public:
    Timer timer;
    int outputFrequency;
    bool verbose;
};

#endif
