#ifndef __EXTRACT_CMD_H__
#define __EXTRACT_CMD_H__

#include "Command.h"
#include "FE_Field.h"


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
    FE_Field *coords;
    std::string output_filename;
};

#endif
