#ifndef __POINTS_READER_H__
#define __POINTS_READER_H__

#include <string>
#include "AnyOption.h"
#include "Reader.h"
#include "Points.h"

class PointsReader
{
public:
    PointsReader();
    ~PointsReader();

public:
    void load_args(AnyOption *opt, const char *opt_prefix);
    void validate_args(const char *cmd_name);

public:
    void load_points();

public:
    std::string pointsOption;
    std::string pointsPath;
    std::string pointsLoc;
    std::string pointsFile;
    std::string pointsExt;

public:
    cigma::Reader *pointsReader;

public:
    cigma::Points *points;

};

#endif
