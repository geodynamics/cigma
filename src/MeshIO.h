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
    cigma::Reader *reader;
    cigma::Writer *writer;

public:
    std::string mesh_path;
    std::string coords_path;
    std::string connect_path;
    cigma::MeshPart *meshPart;

public:
    MeshIO();
    ~MeshIO();
    void load();
    void save();

};


void configure_mesh(AnyOption *opt, MeshIO *meshIO, const char *opt_prefix);


#endif
