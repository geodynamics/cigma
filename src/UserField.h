#ifndef __USER_FIELD_H__
#define __USER_FIELD_H__

#include "Field.h"

namespace cigma
{
    class UserField;
}

class cigma::UserField : public Field
{
public:
    FieldType getType();
};

#endif
