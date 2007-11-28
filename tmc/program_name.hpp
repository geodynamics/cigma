#ifndef GTB_PROGRAM_NAME_INCLUDED
#define GTB_PROGRAM_NAME_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

GTB_BEGIN_NAMESPACE


void set_program_name(const char *argv0);
const char *get_program_name();


GTB_END_NAMESPACE

#endif // GTB_PROGRAM_NAME_INCLUDED
