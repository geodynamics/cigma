#ifndef __COMPARE_CMD_H__
#define __COMPARE_CMD_H__

#include "Command.h"

namespace cigma
{
    class CompareCmdEnv;
    class CompareCmd;
}



/*
 * Environment for CompareCmd
 */
class cigma::CompareCmdEnv : public Environment
{
public:

    CompareCmdEnv();
    ~CompareCmdEnv();

public:

    void configure(const StringMap &cmdline);

public:

    std::string input_filename[2];
    std::string output_filename;

    std::string input_path[2];
    std::string output_path;

    std::string input_name[2];
    std::string output_name;

    std::string quadrature_rule;
    std::string quadrature_order;

};



/*
 * Callback object for `cigma compare [args ...]'
 */
class cigma::CompareCmd : public Command
{
public:

    CompareCmd();
    ~CompareCmd();

public:

    CompareCmdEnv *env;

    void setEnvironment(Environment *env);
    void run();

public:
    //enum types & other typedefs
    
public:
    // internal state
    //MeshPart *meshPart;
    //FEField *field_a;
    //FEField *field_b;
    //QuadratureRule *quadrature;
};


#endif
