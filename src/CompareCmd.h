#ifndef __COMPARE_CMD_H__
#define __COMPARE_CMD_H__

#include <string>
#include "Command.h"

#include "MeshPart.h"
#include "QuadratureRule.h"
#include "Field.h"
#include "Residuals.h"

#include "Timer.h"
#include "Writer.h"
#include "Reader.h"
#include "MeshPartReader.h"
#include "QuadratureReader.h"
#include "FieldReader.h"


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
    void start_timer();
    void update_timer(int e);
    void end_timer();

public:
    QuadraturePoints *quadrature;
    MeshPart *meshPart;
    QuadratureRule *qr;
    Field *field_a;
    Field *field_b;
    Residuals *residuals;

public:
    MeshPartReader meshPartReader;
    QuadratureReader quadratureReader;
    FieldReader firstReader;
    FieldReader secondReader;

    Writer *writer;
    std::string outputPath;

public:
    Timer timer;
    int outputFrequency;
    bool verbose;
};

#endif
