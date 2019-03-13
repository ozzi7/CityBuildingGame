#pragma once
#include <event_visitor.h>

class Event
{
public:
	virtual void Accept(EventVisitor* eventVisitor) = 0;
};