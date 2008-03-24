#ifndef __MESH_PART_READER_H__
#define __MESH_PART_READER_H__

#include <string>
#include "AnyOption.h"
#include "Reader.h"
#include "MeshPart.h"


class MeshPartReader
{
public:
    MeshPartReader();
    ~MeshPartReader();

public:
    void load_args(AnyOption *opt, const char *opt_prefix);
    void validate_args(const char *cmd_name);

public:
    bool has_paths();
    bool has_coords_path();
    bool has_connect_path();
    bool has_valid_paths();
    bool prepare_paths();

public:
    void load_mesh();

public:
    std::string meshPath;
    std::string coordsPath;
    std::string connectPath;

public:
    std::string meshLoc, meshFile, meshExt;
    std::string coordsLoc, coordsFile, coordsExt;
    std::string connectLoc, connectFile, connectExt;

public:
    cigma::Reader *meshReader;
    cigma::Reader *coordsReader;
    cigma::Reader *connectReader;

public:
    cigma::MeshPart *meshPart;

};


#endif
