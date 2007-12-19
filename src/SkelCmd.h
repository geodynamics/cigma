#ifndef __SKEL_CMD_H__
#define __SKEL_CMD_H__

#include <vector>
#include "Command.h"

namespace cigma
{
    class SkelCmd;
}


/**
 * @brief Skeleton class for creating a new command
 *
 */
class cigma::SkelCmd : public Command
{
public:
    SkelCmd();
    ~SkelCmd();

public:
    void setupOptions(AnyOption *opt);
    void configure(AnyOption *opt);
    int run();

public:
    void myMethod1();
    void myMethod2();

public:
    int myVar1;
    double myVar2;
};


#endif
