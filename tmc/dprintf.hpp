#undef DPRINTF
#undef DPRINTF2
#undef DPRINTF3
#undef DPRINTF4
#undef DPRINTF5
#undef DPRINTF6
#undef DPRINTF7

#ifndef GTB_C_STDIO_INCLUDED
#include <stdio.h>
#define GTB_C_STDIO_INCLUDED
#endif

#ifdef DEBUG
#	define DPRINTF(arg) fprintf(stderr, arg)
#	define DPRINTF2(arg1, arg2) fprintf(stderr, arg1, arg2)
#	define DPRINTF3(arg1, arg2, arg3) fprintf(stderr, arg1, arg2, arg3)
#	define DPRINTF4(arg1, arg2, arg3, arg4) \
		fprintf(stderr, arg1, arg2, arg3, arg4)
#	define DPRINTF5(arg1, arg2, arg3, arg4, arg5) \
		fprintf(stderr, arg1, arg2, arg3, arg4, arg5)
#	define DPRINTF6(arg1, arg2, arg3, arg4, arg5, arg6) \
		fprintf(stderr, arg1, arg2, arg3, arg4, arg5, arg6)
#	define DPRINTF7(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
		fprintf(stderr, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#else
#	define DPRINTF(arg)
#	define DPRINTF2(arg1, arg2)
#	define DPRINTF3(arg1, arg2, arg3)
#	define DPRINTF4(arg1, arg2, arg3, arg4)
#	define DPRINTF5(arg1, arg2, arg3, arg4, arg5)
#	define DPRINTF6(arg1, arg2, arg3, arg4, arg5, arg6)
#	define DPRINTF7(arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#endif
