#ifndef GTB_PROGRAM_NAME_INCLUDED
#include "program_name.hpp"
#endif

#ifndef GTB_CXX_STRING_INCLUDED
#include <string>
#define GTB_CXX_STRING_INCLUDED
#endif

#ifndef GTB_C_ASSERT_INCLUDED
#include <assert.h>
#define GTB_C_ASSERT_INCLUDED
#endif

GTB_BEGIN_NAMESPACE


static std::string program_name;


const char *get_program_name()
{
	return program_name.c_str();
}


void set_program_name(const char *argv0)
{
	assert(argv0 != NULL);
	program_name = argv0;
}


GTB_END_NAMESPACE

