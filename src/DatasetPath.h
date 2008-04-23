#ifndef __DATASET_PATH_H__
#define __DATASET_PATH_H__

#include <string>

namespace cigma
{
    class DatasetPath;
}

class cigma::DatasetPath
{
public:
    DatasetPath();
    DatasetPath(std::string path);
    DatasetPath(const char *path);
    ~DatasetPath();

public:
    std::string file;
    std::string location;
};

#endif
