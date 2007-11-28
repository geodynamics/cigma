#ifndef GTB_VERSION_INCLUDED
#include "version.hpp"
#endif

#ifndef GTB_C_STDLIB_INCLUDED
#include <stdlib.h>
#endif

GTB_BEGIN_NAMESPACE


const unsigned major_version = 0;
const unsigned minor_version = 6;
const unsigned micro_version = 27;
const unsigned binary_age =    0;
const unsigned interface_age = 0;


const char *check_version(unsigned required_major,
			  unsigned required_minor,
			  unsigned required_micro)
{
	if (required_major > GTB_MAJOR_VERSION) {
		return "gtb version too old (major mismatch)";
	}
	if ((int) required_major < GTB_MAJOR_VERSION) {
		return "gtb version too new (major mismatch)";
	}
	if (required_minor > GTB_MINOR_VERSION) {
		return "gtb version too old (minor mismatch)";
	}
	if ((int) required_minor < GTB_MINOR_VERSION) {
		return "gtb version too new (minor mismatch)";
	}
	if ((int) required_micro < GTB_MICRO_VERSION - GTB_BINARY_AGE) {
		return "gtb version too new (micro mismatch)";
	}
	if (required_micro > GTB_MICRO_VERSION) {
		return "gtb version too old (micro mismatch)";
	}
	return NULL;
}


GTB_END_NAMESPACE

// Local Variables:
// mode:C++
// End:
