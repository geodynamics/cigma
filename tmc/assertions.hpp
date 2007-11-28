#ifndef GTB_ASSERTIONS_INCLUDED
#define GTB_ASSERTIONS_INCLUDED

#ifndef GTB_ERRORS_INCLUDED
#include "errors.hpp"
#endif

#ifdef GTB_DISABLE_ASSERTIONS

#define GTB_REQUIRE(condition)
#define GTB_ENSURE(condition)
#define GTB_CHECK(condition)
#define GTB_INVARIANT(condition)

#else // GTB_DISABLE_ASSERTIONS

#define GTB_REQUIRE(condition) \
do { \
        if (!(condition)) { \
                ::gtb::error(__FILE__, __LINE__, \
			     "precondition violation: %s", # condition); \
        } \
} while (0)


#define GTB_ENSURE(condition) \
do { \
        if (!(condition)) { \
                ::gtb::error(__FILE__, __LINE__, \
			     "postcondition violation: %s", # condition); \
        } \
} while (0)

#define GTB_CHECK(condition) \
do { \
        if (!(condition)) { \
                ::gtb::error(__FILE__, __LINE__, \
			     "check failure: %s", # condition); \
        } \
} while (0)

#define GTB_INVARIANT(condition) \
do { \
        if (!(condition)) { \
                ::gtb::error(__FILE__, __LINE__, \
			     "invariant violation: %s", # condition); \
        } \
} while (0)

#endif // GTB_DISABLE_ASSERTIONS

#endif // GTB_ASSERTIONS_INCLUDED
