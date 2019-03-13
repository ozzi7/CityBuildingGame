#pragma once
#include <event.h>

class MoveEvent : public Event
{
public:
	MoveEvent(int aFromX, int aFromY, int aToX, int aToY, int aIndex);
	void MoveEvent::Accept(EventVisitor* eventVisitor);

	int fromX;
	int fromY;

	int toX;
	int toY;

	int index;
};