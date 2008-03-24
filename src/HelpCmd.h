#ifndef __HELP_CMD_H__
#define __HELP_CMD_H__

#include <vector>

#include "Command.h"
#include "CommandSet.h"


namespace cigma
{
    class HelpCmd;
}


/**
 * @brief Callback object for `cigma help [args ...]'
 *
 */
class cigma::HelpCmd : public Command
{
public:
    HelpCmd();
    ~HelpCmd();

public:
    void setupOptions(AnyOption *opt);
    void configure(AnyOption *opt);
    int run();

public:
    void setCommandSet(CommandSet *cmdset);

public:
    CommandSet *cmdset;
    std::string subcommand;
    std::vector<std::string> usageList;
};


#endif
