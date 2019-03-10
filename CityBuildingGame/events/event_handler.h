#pragma once
#include <concurrentqueue.h>
#include <event.h>

class EventHandler {
public:
	void AddEvent(Event*);
	Event * GetEvent();
private:
	moodycamel::ConcurrentQueue<Event*> cq;
};
