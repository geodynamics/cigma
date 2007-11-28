#ifndef GTB_OBSERVER_INCLUDED
#define GTB_OBSERVER_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

GTB_BEGIN_NAMESPACE


class Observer {
public:
	virtual ~Observer();

	virtual void update() = 0;
};


GTB_END_NAMESPACE

#endif // GTB_OBSERVER_INCLUDED
