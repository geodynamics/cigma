#ifndef GTB_VERSION_INCLUDED
#define GTB_VERSION_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

GTB_BEGIN_NAMESPACE


// run time check
extern const unsigned major_version;
extern const unsigned minor_version;
extern const unsigned micro_version;
extern const unsigned binary_age;
extern const unsigned interface_age;
const char *check_version(unsigned required_major,
			  unsigned required_minor,
			  unsigned required_micro);

GTB_END_NAMESPACE


// compile time check
#define GTB_MAJOR_VERSION				(0)
#define GTB_MINOR_VERSION				(6)
#define GTB_MICRO_VERSION				(27)
#define GTB_BINARY_AGE					(0)
#define GTB_INTERFACE_AGE				(0)
#define	GTB_CHECK_VERSION(major,minor,micro)	\
    (GTB_MAJOR_VERSION > (major) || \
     (GTB_MAJOR_VERSION == (major) && GTB_MINOR_VERSION > (minor)) || \
     (GTB_MAJOR_VERSION == (major) && GTB_MINOR_VERSION == (minor) && \
      GTB_MICRO_VERSION >= (micro)))


#endif // GTB_VERSION_INCLUDED

// Local Variables:
// mode:C++
// End:
