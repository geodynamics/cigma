#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <string>
#include <vector>


/*
 * string functions
 */

void string_to_int(const std::string &str, int &val);

void string_to_double(const std::string &str, double &val);

void string_to_paths(const std::string &paths,
                     std::vector<std::string> &filepaths);

void string_tokenize(const std::string &str,
                     std::vector<std::string> &tokens,
                     const std::string &delimiters);


/*
 * path functions
 */

void path_dirname(const std::string &filepath,
                  std::string &dirname);

void path_basename(const std::string &filepath,
                   std::string &basename);

void path_splitext(const std::string &filepath,
                   std::string &root,
                   std::string &ext);

int parse_path_pair(const std::string &str,
                    std::string &path1,
                    std::string &path2);

void parse_dataset_path(const std::string &str,
                        std::string &dataset,
                        std::string &filepath,
                        std::string &extension);

#endif
