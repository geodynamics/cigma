#ifndef GTB_ERRORS_INCLUDED
#define GTB_ERRORS_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_C_STRING_INCLUDED
#include <string.h>
#define GTB_C_STRING_INCLUDED
#endif

GTB_BEGIN_NAMESPACE


void error(const char *file, int line, const char *fmt, ...);
void warning(const char *file, int line, const char *fmt, ...);


GTB_END_NAMESPACE


#define GTB_ERROR(arg) \
	::gtb::error(__FILE__, __LINE__, arg)
#define GTB_ERROR2(arg1, arg2) \
	::gtb::error(__FILE__, __LINE__, arg1, arg2)
#define GTB_ERROR3(arg1, arg2, arg3) \
	::gtb::error(__FILE__, __LINE__, arg1, arg2, arg3)
#define GTB_ERROR4(arg1, arg2, arg3, arg4) \
	::gtb::error(__FILE__, __LINE__, arg1, arg2, arg3, arg4)
#define GTB_ERROR5(arg1, arg2, arg3, arg4, arg5) \
	::gtb::error(__FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5)
#define GTB_ERROR6(arg1, arg2, arg3, arg4, arg5, arg6) \
	::gtb::error(__FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6)


#define GTB_WARNING(arg) \
	::gtb::warning(__FILE__, __LINE__, arg)
#define GTB_WARNING2(arg1, arg2) \
	::gtb::warning(__FILE__, __LINE__, arg1, arg2)
#define GTB_WARNING3(arg1, arg2, arg3) \
	::gtb::warning(__FILE__, __LINE__, arg1, arg2, arg3)
#define GTB_WARNING4(arg1, arg2, arg3, arg4) \
	::gtb::warning(__FILE__, __LINE__, arg1, arg2, arg3, arg4)
#define GTB_WARNING5(arg1, arg2, arg3, arg4, arg5) \
	::gtb::warning(__FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5)
#define GTB_WARNING6(arg1, arg2, arg3, arg4, arg6) \
	::gtb::warning(__FILE__, __LINE__, arg1, arg2, arg3, arg4, arg5, arg6)


#define GTB_ERROR_STATUS(msg, status) \
	::gtb::error(__FILE__, __LINE__, "%s: %s", msg, strerror(status))


#endif // GTB_ERRORS_INCLUDED
