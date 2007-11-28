#ifndef GTB_MATH_UTIL_INCLUDED
#include "math_util.hpp"
#endif

#include "gtb_config.h"

#ifdef HAVE_VALUES_H
#include <values.h>
#endif

const double GTB_E = 2.7182818284590452354;
const double GTB_LOG2E = 1.4426950408889634074;
const double GTB_LOG10E = 0.43429448190325182765;
const double GTB_LN2 = 0.69314718055994530942;
const double GTB_LN10 = 2.30258509299404568402;
const double GTB_PI = 3.14159265358979323846;
const double GTB_PI_2 = 1.57079632679489661923;
const double GTB_PI_4 = 0.78539816339744830962;
const double GTB_1_PI = 0.31830988618379067154;
const double GTB_2_PI = 0.63661977236758134308;
const double GTB_2_SQRTPI = 1.12837916709551257390;
const double GTB_SQRT2 = 1.41421356237309504880;
const double GTB_SQRT1_2 = 0.70710678118654752440;
const double GTB_PI_OVER_180 = 0.01745329251994329576;
const double GTB_180_OVER_PI = 57.29577951308232087684;
const double GTB_CM_PER_INCH = 2.54;
const double GTB_MM_PER_INCH = 25.4;
const unsigned GTB_POINTS_PER_INCH = 72;

GTB_BEGIN_NAMESPACE


// Computing (A * B) mod N Efficiently in ANSI C
// Henry G. Baker
// http://linux.rice.edu/~rahul/hbaker/AB-mod-N.html
unsigned ab_mod_n(unsigned a, unsigned b, unsigned n)
{
	//fprintf(stderr, "a: %d  b: %d  n: %d\n", a, b, n);
	double dn = n;		// 31 bits of n > 0
	double da = a;		// 31 bits of a >= 0
	double db = b;		// 31 bits of b >= 0
	double dp = da * db;	// ~53 bits of a * b
	double dq = dp / dn;	// ~51 bits of a * b / n
	unsigned long qpp = (unsigned long) (dq + 0.5);	// q''=round(q)
	unsigned long rp = a * b - qpp * n;	// r'
	unsigned r = (rp & 0x80000000) ? (rp + n) : rp;
	return r;
}


double drand()
{
#ifdef HAVE_DRAND48
	return drand48();
#else
	return (double) rand() / (double) INT_MAX;
#endif
}


GTB_END_NAMESPACE
