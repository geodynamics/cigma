#ifndef __EXTRACT_CMD_H__
#define __EXTRACT_CMD_H__

#include "Command.h"
#include "FE_Field.h"
#include "MeshIO.h"
#include "QuadratureIO.h"
#include "Writer.h"

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
    MeshIO meshIO;
    QuadratureIO quadratureIO;
    std::string output_filename;
    std::string points_path;
    bool verbose;

public:
    FE_Field *coordsField;
    Writer *writer;

};

#endif
