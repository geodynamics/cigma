#ifndef GTB_SUBJECT_INCLUDED
#define GTB_SUBJECT_INCLUDED

#ifndef GTB_NAMESPACE_INCLUDED
#include "namespace.hpp"
#endif

#ifndef GTB_CXX_LIST_INCLUDED
#include <list>
#define GTB_CXX_LIST_INCLUDED
#endif

GTB_BEGIN_NAMESPACE

class Observer;


class Subject {
public:
	virtual ~Subject();

	void attach(Observer &observer);
	void detach(Observer &observer);
	void notify();

protected:
	std::list<Observer *> m_observers;
};


GTB_END_NAMESPACE

#endif // GTB_SUBJECT_INCLUDED
