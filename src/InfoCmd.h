#ifndef __INFO_CMD_H__
#define __INFO_CMD_H__

#include <string>
#include "Command.h"
#include "MeshPart.h"
#include "MeshPartReader.h"
#include "Field.h"
#include "FieldReader.h"
#include "HdfWriter.h"

namespace cigma
{
    class InfoCmd;
}

class cigma::InfoCmd : public Command
{
public:
    InfoCmd();
    ~InfoCmd();

public:
    void setupOptions(AnyOption *opt);
    void configure(AnyOption *opt);
    int run();

public:
    std::string outputFilename;

public:
    FieldReader fieldReader;
    MeshPartReader meshPartReader;
    HdfWriter hdfWriter;

public:
    Field *field;
    MeshPart *meshPart;
};

#endif
