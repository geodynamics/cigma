#ifndef __FIELD_READER_H__
#define __FIELD_READER_H__

#include <string>
#include "AnyOption.h"
#include "MeshPartReader.h"
#include "PointsReader.h"
#include "Reader.h"
#include "Field.h"
#include "FieldSet.h"

class FieldReader
{
public:
    FieldReader();
    ~FieldReader();
    void setFieldSet(cigma::FieldSet *fs);

public:
    void load_args(AnyOption *opt, const char *opt_prefix);
    void validate_args(const char *cmd_name);
    bool fieldPathIsZero();

public:
    void load_field();

public:
    std::string fieldOption;
    std::string fieldPath;
    bool verbose;

public:
    MeshPartReader meshPartReader;
    PointsReader pointsReader;
    PointsReader valuesReader;
    cigma::Reader *dofsReader;

public:
    cigma::Field *field;
    cigma::FieldSet *fieldSet;
};

#endif
