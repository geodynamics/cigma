#ifndef __SEARCH_CMD_H__
#define __SEARCH_CMD_H__

#include <string>
#include "Command.h"


#include "Points.h"
#include "MeshPart.h"
#include "FE_Field.h"

#include "MeshPartReader.h"
#include "PointsReader.h"
#include "HdfWriter.h"
#include "TextWriter.h"

namespace cigma
{
    class SearchCmd;
}

/**
 * @brief Callback object for `cigma search [args ...]'
 */
class cigma::SearchCmd : public Command
{
public:
    SearchCmd();
    ~SearchCmd();

public:
    void setupOptions(AnyOption *opt);
    void configure(AnyOption *opt);
    int run();

public:
    Points *points;
    MeshPart *meshPart;
    FE_Field *coordsField;

public:
    PointsReader pointsReader;
    MeshPartReader meshPartReader;
    Writer *writer;
    std::string outputFile;
    bool verbose;
};

#endif
