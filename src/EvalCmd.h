#ifndef __EVAL_CMD_H__
#define __EVAL_CMD_H__

#include <string>
#include "Command.h"
#include "Field.h"
#include "Points.h"
#include "FieldReader.h"
#include "PointsReader.h"
#include "Writer.h"

namespace cigma
{
    class EvalCmd;
}

/**
 * @brief Callback object for `cigma eval [options]'
 */
class cigma::EvalCmd : public Command
{
public:
    EvalCmd();
    ~EvalCmd();

public:
    void setupOptions(AnyOption *opt);
    void configure(AnyOption *opt);
    int run();

public:
    FieldReader fieldReader;
    PointsReader pointsReader;
    Writer *valuesWriter;
    std::string valuesLoc;
    std::string valuesFile;
    bool verbose;

public:
    Field *field;
    Points *points;
    Points *values;
};

#endif
