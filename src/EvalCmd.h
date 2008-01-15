#ifndef __EVAL_CMD_H__
#define __EVAL_CMD_H__

#include "Command.h"
#include "FE_Field.h"
#include "Points.h"

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
    FE_Field *field;
    Points *points;
    std::string output_filename;
};

#endif
