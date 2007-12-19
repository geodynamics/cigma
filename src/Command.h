#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <map>

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
    typedef std::map<std::string, std::string> StringMap;
    typedef std::vector<std::string> StringVector;

public:
    virtual ~Command();

public:
    //virtual void parse(int argc, char *argv[], StringMap &params) = 0;
    //virtual void configure(const StringMap &params) = 0;
    virtual void configure(int argc, char *argv[]) = 0;
    virtual void help() = 0;
    virtual void run() = 0;

public:
    std::string name;
};


#endif
