#ifndef GTB_ERRORS_INCLUDED
#include "errors.hpp"
#endif

#ifndef GTB_PROGRAM_NAME_INCLUDED
#include "program_name.hpp"
#endif

#ifndef GTB_C_ERRNO_INCLUDED
#include <errno.h>
#define GTB_C_ERRNO_INCLUDED
#endif

#ifndef GTB_C_STDARG_INCLUDED
#include <stdarg.h>
#define GTB_C_STDARG_INCLUDED
#endif

#ifndef GTB_C_STDIO_INCLUDED
#include <stdio.h>
#define GTB_C_STDIO_INCLUDED
#endif

#ifndef GTB_C_STDLIB_INCLUDED
#include <stdlib.h>
#define GTB_C_STDLIB_INCLUDED
#endif

GTB_BEGIN_NAMESPACE


void error(const char *file, int line, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	if (get_program_name != NULL) {
		fprintf(stderr, "%s: ", get_program_name());
	}
	fprintf(stderr, "ERROR: %s:%d: ", file, line);
	vfprintf(stderr, fmt, args);
	va_end(args);
	if (errno != 0) {
		fprintf(stderr, ": ");
		perror(NULL);
	} else {
		fprintf(stderr, "\n");
	}
	abort();
}


void warning(const char *file, int line, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	if (get_program_name() != NULL) {
		fprintf(stderr, "%s: ", get_program_name());
	}
	fprintf(stderr, "WARNING: %s:%d: ", file, line);
	vfprintf(stderr, fmt, args);
	va_end(args);
	if (errno != 0) {
		fprintf(stderr, ": ");
		perror(NULL);
	} else {
		fprintf(stderr, "\n");
	}
}


GTB_END_NAMESPACE
