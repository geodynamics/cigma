#ifndef __FIELD_IO_H__
#define __FIELD_IO_H__

#include <string>
#include "MeshIO.h"
#include "QuadratureIO.h"
#include "FE_Field.h"

class FieldIO
{
public:
    cigma::Reader *reader;
    cigma::Writer *writer;

public:
    std::string field_path;
    cigma::FE_Field *field;
    MeshIO meshIO;
    QuadratureIO quadratureIO;

public:
    FieldIO();
    ~FieldIO();
    void load();
    void save();
};


void load_args(AnyOption *opt, FieldIO *fieldIO, const char *opt_prefix);


#endif
