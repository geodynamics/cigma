#ifndef __EVAL_CMD_H__
#define __EVAL_CMD_H__

#include "Command.h"

#include "Field.h"
#include "Points.h"

//#include "FieldIO.h"
//#include "PointsIO.h"


namespace cigma
{
    class EvalCmd;
}


/**
 * @brief Callback object for `cigma eval [args ...]'
 *
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
    FieldIO fieldIO;

    Reader *points_reader;
    std::string points_loc;
    std::string points_file;

    Writer *values_writer;
    std::string values_loc;
    std::string values_file;

    bool verbose;

public:
    Field *field;
    Points *points;
    Points *values;
};

#endif
