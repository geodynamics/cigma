#ifndef GTB_EPSILON_INCLUDED
#define GTB_EPSILON_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

GTB_BEGIN_NAMESPACE


bool eps_is_zero(double x, double eps);
bool eps_is_positive(double x, double eps);
bool eps_is_positive_or_zero(double x, double eps);
bool eps_is_negative(double x, double eps);
bool eps_is_negative_or_zero(double x, double eps);

bool eps_is_equal(double x, double y, double eps);
bool eps_is_greater(double x, double y, double eps);
bool eps_is_greater_or_equal(double x, double y, double eps);
bool eps_is_less(double x, double y, double eps);
bool eps_is_less_or_equal(double x, double y, double eps);


inline bool eps_is_zero(double x, double eps)
{
	return (x > -eps) && (x < eps);
}


inline bool eps_is_positive(double x, double eps)
{
	return x >= eps;
}


inline bool eps_is_positive_or_zero(double x, double eps)
{
	return x > -eps;
}


inline bool eps_is_negative(double x, double eps)
{
	return (x <= -eps);
}


inline bool eps_is_negative_or_zero(double x, double eps)
{
	return x < eps;
}


inline bool eps_is_equal(double x, double y, double eps)
{
	return eps_is_zero(x - y, eps);
}


inline bool eps_is_greater(double x, double y, double eps)
{
	return eps_is_positive(x - y, eps);
}


inline bool eps_is_greater_or_equal(double x, double y, double eps)
{
	return eps_is_positive_or_zero(x - y, eps);
}


inline bool eps_is_less(double x, double y, double eps)
{
	return eps_is_negative(x - y, eps);
}


inline bool eps_is_less_or_equal(double x, double y, double eps)
{
	return eps_is_negative_or_zero(x - y, eps);
}


GTB_END_NAMESPACE

#endif // GTB_EPSILON_INCLUDED
