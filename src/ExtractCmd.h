#ifndef __EXTRACT_CMD_H__
#define __EXTRACT_CMD_H__

#include <string>
#include "Command.h"
#include "FE_Field.h"
#include "MeshPartReader.h"
#include "QuadratureReader.h"
#include "Writer.h"

namespace cigma
{
    class ExtractCmd;
}

/**
 * @brief Callback object for `cigma extract [options]'
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
    MeshPartReader meshPartReader;
    QuadratureReader quadratureReader;
    std::string outputFilename;
    std::string pointsPath;
    bool verbose;

public:
    FE_Field *coordsField;
    Writer *pointsWriter;

};


#endif
