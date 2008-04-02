#ifndef __FIELD_SET_H__
#define __FIELD_SET_H__

#include <map>
#include <string>
#include "Field.h"

namespace cigma
{
    class FieldSet;
}

class cigma::FieldSet
{
public:
    typedef std::map<std::string,Field*> FieldMap;

public:
    FieldSet();
    ~FieldSet();

public:
    void initialize();

public:
    void addField(std::string name, Field *field);
    Field *getField(std::string name);

public:
    FieldMap fields;

};

#endif
