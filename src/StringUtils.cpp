#include <string>
#include <vector>
#include <cassert>
#include "StringUtils.h"
#include "convert.h"


void string_to_int(const std::string &str, int &val)
{
    try
    {
        val = convertTo<int>(str);
    }
    catch(BadConversion &exc)
    {
        val = 0;
    }
}


void string_to_double(const std::string &str, double &val)
{
    try
    {
        val = convertTo<double>(str);
    }
    catch(BadConversion &exc)
    {
        val = 0.0;
    }
}


void string_to_paths(const std::string &paths,
                     std::vector<std::string> &filepaths)
{
    filepaths.clear();
    string_tokenize(paths, filepaths, ":");
}


void string_tokenize(const std::string &str,
                     std::vector<std::string> &tokens,
                     const std::string &delimiters = " ")
{
    // Skip delimiters at beginning
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

    // Find first "non-delimiter"
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector
        tokens.push_back(str.substr(lastPos, pos-lastPos));

        // Skip delimiters. Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);

        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


void path_dirname(const std::string &filepath,
                  std::string &dirname)
{
    std::string::size_type lastSep = filepath.find_last_of("/");
    if (lastSep != std::string::npos)
    {
        dirname = filepath.substr(0, lastSep + 1);
    }
    else
    {
        dirname = "";
    }
}


void path_basename(const std::string &filepath,
                   std::string &basename)
{
    std::string::size_type lastSep = filepath.find_last_of("/");
    basename = filepath.substr(lastSep + 1);
}


void path_splitext(const std::string &filepath,
                   std::string &root,
                   std::string &ext)
{
    std::string::size_type lastDot = filepath.find_last_of(".");
    std::string::size_type lastSep = filepath.find_last_of("/");

    if ((lastDot != std::string::npos) &&
        ((lastSep == std::string::npos) || (lastSep < lastDot)))
    {
        root = filepath.substr(0, lastDot);
        ext = filepath.substr(lastDot);
    }
    else
    {
        root = filepath;
        ext = "";
    }
}


int parse_path_pair(const std::string &str,
                    std::string &path1,
                    std::string &path2)
{
    std::vector<std::string> paths;

    string_to_paths(str, paths);

    assert(paths.size() > 0);

    if (paths.size() == 1)
    {
        path1 = str;
        path2 = "";
        return 2;
    }
    else if (paths.size() == 2)
    {
        std::vector<std::string>::iterator it;
        it = paths.begin();
        path1 = *it;
        ++it;
        path2 = *it;
        return 2;
    }
    else
    {
        path1 = "";
        path2 = "";
    }
    return paths.size();
}

void parse_dataset_path(const std::string &str,
                        std::string &dataset,
                        std::string &filepath,
                        std::string &extension)
{
    std::string fileroot;
    std::vector<std::string> paths;
    parse_path_pair(str, filepath, dataset);
    path_splitext(filepath, fileroot, extension);
}
