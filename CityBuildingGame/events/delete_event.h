#pragma once
#include <event.h>

class GameObject;

class DeleteEvent : public Event
{
public:
	DeleteEvent(int posX, int posY, GameObject * aGameObject);
	void DeleteEvent::Accept(EventVisitor* eventVisitor);

	int posX;
	int posY;

	GameObject * gameObject;
};