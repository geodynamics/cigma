#ifndef GTB_SUBJECT_INCLUDED
#include "Subject.hpp"
#endif

#ifndef GTB_OBSERVER_INCLUDED
#include "Observer.hpp"
#endif

#ifndef GTB_CXX_ALGORITHM_INCLUDED
#include <algorithm>
#define GTB_CXX_ALGORITHM_INCLUDED
#endif

#ifndef GTB_ASSERTIONS_INCLUDED
#include "assertions.hpp"
#endif

GTB_BEGIN_NAMESPACE


Subject::~Subject()
{
}


void Subject::attach(Observer &observer)
{
	std::list<Observer *>::const_iterator i = std::find(m_observers.begin(),
						            m_observers.end(),
						            &observer);
	if (i == m_observers.end()) {
		m_observers.push_back(&observer);
	}
}


void Subject::detach(Observer &observer)
{
	std::list<Observer *>::iterator i = std::find(m_observers.begin(),
                                                      m_observers.end(),
						      &observer);
	if (i == m_observers.end()) {
		GTB_ERROR("observer not found");
	}
	m_observers.erase(i);
}


void Subject::notify()
{
	for (std::list<Observer *>::const_iterator i = m_observers.begin();
	     i != m_observers.end();
	     ++i) {
		Observer *observer = *i;
		GTB_CHECK(observer != NULL);
		observer->update();
	}
}


GTB_END_NAMESPACE
