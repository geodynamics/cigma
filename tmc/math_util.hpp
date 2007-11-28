#ifndef GTB_MATH_UTIL_INCLUDED
#define GTB_MATH_UTIL_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_C_MATH_INCLUDED
#include <math.h>
#define GTB_C_MATH_INCLUDED
#endif

#ifndef GTB_C_STDLIB_INCLUDED
#include <stdlib.h>
#define GTB_C_STDLIB_INCLUDED
#endif

#include <algorithm>

extern const double GTB_E;
extern const double GTB_LOG2E;
extern const double GTB_LOG10E;
extern const double GTB_LN2;
extern const double GTB_LN10;
extern const double GTB_PI;
extern const double GTB_PI_2;
extern const double GTB_PI_4;
extern const double GTB_1_PI;
extern const double GTB_2_PI;
extern const double GTB_2_SQRTPI;
extern const double GTB_SQRT2;
extern const double GTB_SQRT1_2;
extern const double GTB_PI_OVER_180;
extern const double GTB_180_OVER_PI;
extern const double GTB_CM_PER_INCH;
extern const double GTB_MM_PER_INCH;
extern const unsigned GTB_POINTS_PER_INCH;

GTB_BEGIN_NAMESPACE


double deg_to_rad(double x);
double rad_to_deg(double x);
double log2(double x);
double power_of_two_ceiling(double x);
unsigned ab_mod_n(unsigned a, unsigned b, unsigned n);
double drand();
unsigned factorial(unsigned x);
int rand32();

template <class T> T abs(T x);
template <class T> T clamp(T x, T low, T high);
template <class T> T max3(const T &a, const T &b, const T &c);
template <class T> T max3(const T &x, const T &y, const T &z, int &k);
template <class T> T min3(const T &a, const T &b, const T &c);
template <class T> T min3(const T &x, const T &y, const T &z, int &k);
template <class T> T ipow(T x, int exponent);
template <class T> T sqr(const T &x);
template<class T> void assign_if_less(T &x, const T &y);
template<class T> unsigned logn(T v);
template<class T> T pwr2(T v);
template<class T> T round2up(T v);


inline double deg_to_rad(double x)
{
	return x * GTB_PI_OVER_180;
}


inline double rad_to_deg(double x)
{
	return x * GTB_180_OVER_PI;
}


inline double log2(double x)
{
        return log10(x) / log10(2.0);
}


inline double power_of_two_ceiling(double x)
{
        return pow((double)2, ceil(log2(x)));
}


inline unsigned factorial(unsigned x)
{
	unsigned result = 1;
	while (x) {
		result *= x;
		--x;
	}
	return result;
}


inline int rand32()
{
	return (rand() << 15) + rand();
}


template <class T>
inline T abs(T x)
{
	return (x >= 0) ? x : -x;
}


template <class T>
inline T clamp(T x, T low, T high)
{
	if (x < low) {
		return low;
	} else if (x > high) {
		return high;
	} else {
		return x;
	}
}


template <class T>
inline T max3(const T &a, const T &b, const T &c)
{
	return std::max(std::max(a, b), c);
}


// Similar to max3.  k is the index [0..2] of the largest item.
template <class T>
inline T max3(const T &x, const T &y, const T &z, int &k)
{
	if (x > y) {
		if (x > z) {
			k = 0;
			return x;
		} else {
			k = 2;
			return z;
		}
	} else if (y > z) {
		k = 1;
		return y;
	} else {
		k = 2;
		return z;
	}
}


template <class T>
inline T min3(const T &a, const T &b, const T &c)
{
	return std::min(std::min(a, b), c);
}


// Similar to max3.  k is the index [0..2] of the largest item.
template <class T>
inline T min3(const T &x, const T &y, const T &z, int &k)
{
	if (x < y) {
		if (x < z) {
			k = 0;
			return x;
		} else {
			k = 2;
			return z;
		}
	} else if (y < z) {
		k = 1;
		return y;
	} else {
		k = 2;
		return z;
	}
}


template <class T>
inline T ipow(T x, int exponent)
{
	T r=1;
	while (exponent--) {
		r *= x;
	}
	return r;
}


template <class T>
inline T sqr(const T &x)
{
	return x * x;
}


template<class T>
inline void assign_if_less(T &x, const T &y)
{
	if (x < y) {
		x = y;
	}
}


// returns the number of bits required to store v
template<class T>
inline unsigned logn(T v)
{
	T last_low_bit = 0;
	T add1 = 0;
	T result = 0;

	while (v) {
		add1 |= last_low_bit;
		last_low_bit = v & 1;
		v >>= 1;
		++result;
	}
	return result - 1 + add1;
}


// returns v to the power of 2
template<class T>
inline T pwr2(T v)
{
	T result = 1;

	while (v) {
		result <<= 1;
		--v;
	}
	return result;
}


// returns v rounded up to the nearst power of 2
template<class T>
inline T round2up(T v)
{
	return pwr2(logn(v));
}


GTB_END_NAMESPACE

#endif // GTB_MATH_UTIL_INCLUDED
