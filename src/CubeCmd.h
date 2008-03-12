#ifndef __CUBE_CMD_H__
#define __CUBE_CMD_H__

#include "Command.h"
#include "CubeMeshPart.h"
#include "Writer.h"

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
    bool verbose;
    std::string output_filename;
    std::string coords_path;
    std::string connect_path;

public:
    int L, M, N;
    CubeMeshPart *mesh;
    Writer *writer;

};

#endif
