#ifndef __CUBE_CMD_H__
#define __CUBE_CMD_H__

#include "Command.h"
#include "CubeMeshPart.h"

namespace cigma
{
    class CubeCmd;
}

class cigma::CubeCmd : public Command
{
public:
    CubeCmd();
    ~CubeCmd();

public:
    void setupOptions(AnyOption *opt);
    void configure(AnyOption *opt);
    int run();

public:
    int L, M, N;
    CubeMeshPart *mesh;
    std::string output_filename;
};

#endif
