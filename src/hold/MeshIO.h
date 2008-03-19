#ifndef __MESH_IO_H__
#define __MESH_IO_H__

#include <string>
#include "AnyOption.h"
#include "Reader.h"
#include "Writer.h"
#include "MeshPart.h"

class MeshIO
{
public:
    std::string mesh_path;
    std::string coords_path;
    std::string connect_path;

public:
    std::string mesh_loc, mesh_file, mesh_ext;
    std::string coords_loc, coords_file, coords_ext;
    std::string connect_loc, connect_file, connect_ext;
    bool prepare();

public:
    cigma::Reader *reader;
    cigma::Reader *coords_reader;
    cigma::Reader *connect_reader;
    cigma::Writer *writer;
    cigma::Writer *coords_writer;
    cigma::Writer *connect_writer;
    cigma::MeshPart *meshPart;

public:
    MeshIO();
    ~MeshIO();

public:
    void load();
    void save();

public:
    bool has_paths();
    bool has_coords_path();
    bool has_connect_path();
    bool has_valid_paths();
};



void load_args(AnyOption *opt, MeshIO *meshIO, const char *opt_prefix);
void validate_args(MeshIO *meshIO, const char *cmd_name);


#endif
