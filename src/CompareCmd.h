#ifndef __COMPARE_CMD_H__
#define __COMPARE_CMD_H__

#include "Command.h"

namespace cigma
{
    class CompareCmd;
}


/*
 * Callback object for `cigma compare [args ...]'
 */
class cigma::CompareCmd : public Command
{
public:
    CompareCmd();
    ~CompareCmd();

public:
    void configure(const StringMap &cmdlineParams);
    void run();

public:
    // internal state
    //MeshPart *meshPart;
    //FEField *field_a;
    //FEField *field_b;
    //QuadratureRule *quadrature;
};


#endif
