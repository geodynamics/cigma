#ifndef __FIELD_READER_H__
#define __FIELD_READER_H__

#include <string>
#include "AnyOption.h"
#include "MeshPartReader.h"
#include "QuadratureReader.h"
#include "Reader.h"
#include "Field.h"

class FieldReader
{
public:
    FieldReader();
    ~FieldReader();

public:
    void load_args(AnyOption *opt, const char *opt_prefix);
    void validate_args(const char *cmd_name);

public:
    void load_field();

public:
    std::string fieldOption;
    std::string fieldPath;

public:
    MeshPartReader meshPartReader;
    QuadratureReader qptsReader;
    cigma::Reader *dofsReader;

public:
    cigma::Field *field;

};

#endif
