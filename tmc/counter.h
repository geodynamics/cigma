#ifndef __COUNTER_H
#define __COUNTER_H

#include "common.hpp"

GTB_BEGIN_NAMESPACE

/*
 * Copyright (c) 1999
 * Shachar Fleishman shacharf@math.tau.ac.il
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

/*
 * A thread safe counter.
 * A class that behaves like int with atomic ++,-- operators.
 */

#ifdef WIN32
class Counter
{
public:
	typedef LONG value_type;

	Counter(value_type v) : m_v(v) {}

	operator value_type() { return m_v; }
	value_type operator++() { return InterlockedIncrement(&m_v); }
	value_type operator++(int) { return InterlockedExchangeAdd(&m_v, 1); }
	value_type operator--() { return InterlockedDecrement(&m_v); }
	value_type operator--(int) { return InterlockedExchangeAdd(&m_v, -1); }

protected:
	value_type m_v;
};
#else
typedef int Counter;
#endif // WIN32

GTB_END_NAMESPACE

#endif // __COUNTER_H

// Local Variables:
// mode:C++
// End:
