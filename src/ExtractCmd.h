#ifndef __EXTRACT_CMD_H__
#define __EXTRACT_CMD_H__

#include "Command.h"
#include "QuadratureRule.h"
#include "MeshPart.h"


namespace cigma
{
    class ExtractCmd;
}


/**
 * @brief Callback object for `cigma extract [args ...]'
 *
 */
class cigma::ExtractCmd : public Command
{
public:
    ExtractCmd();
    ~ExtractCmd();

public:
    void setupOptions(AnyOption *opt);
    void configure(AnyOption *opt);
    int run();

public:
    MeshPart *meshPart;
    QuadratureRule *quadrature;
};

#endif
