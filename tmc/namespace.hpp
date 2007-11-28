#ifndef GTB_NAMESPACE_INCLUDED
#define GTB_NAMESPACE_INCLUDED

#ifdef WIN32
#include "windows.hpp"
#endif

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define GTB_BEGIN_NAMESPACE namespace gtb {
#define GTB_END_NAMESPACE }

#endif // GTB_NAMESPACE_INCLUDED
