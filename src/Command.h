#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <map>

#include "AnyOption.h"


namespace cigma
{
    class Command;
}


/**
 * @brief Base class for commands
 *
 */
class cigma::Command
{
public:
    virtual ~Command();

public:
    virtual void setupOptions(AnyOption *opt) = 0;

public:
    virtual bool helpFlag(AnyOption *opt);
    virtual void configure(AnyOption *opt) = 0;
    virtual int run() = 0;

public:
    std::string name;
};


#endif
