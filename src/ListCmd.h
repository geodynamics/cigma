#ifndef __LIST_CMD_H__
#define __LIST_CMD_H__

#include <string>
#include "Command.h"

namespace cigma
{
    class ListCmd;
}


/**
 * @brief Class for examining cigma's input and output files.
 *
 */
class cigma::ListCmd : public Command
{
public:
    ListCmd();
    ~ListCmd();

public:
    void setupOptions(AnyOption *opt);
    void configure(AnyOption *opt);
    int run();

public:
    std::string filename;
};

#endif
