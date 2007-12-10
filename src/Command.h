#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <map>

namespace cigma
{
    class Environment;
    class Command;
}



/*
 * Base class for runtime enviroment
 */
class cigma::Environment
{
public:
    typedef std::map<std::string, std::string> StringMap;
public:
    Environment();
    virtual ~Environment();
    virtual void configure(const StringMap &cmdline) = 0;
public:
    bool configured;
};


/*
 * Base class for commands
 */
class cigma::Command
{
public:
    virtual void setEnvironment(Environment *env) = 0;
    virtual void run() = 0;
    virtual ~Command();
};


#endif
