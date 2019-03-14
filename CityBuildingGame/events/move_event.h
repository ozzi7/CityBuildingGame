#pragma once
#include <event.h>

class GameObject;

class MoveEvent : public Event
{
public:
	MoveEvent(int aFromX, int aFromY, int aToX, int aToY, GameObject * aGameObject);
	void MoveEvent::Accept(EventVisitor* eventVisitor);

	int fromX;
	int fromY;

	int toX;
	int toY;

	GameObject * gameObject;
};