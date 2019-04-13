#pragma once
#include <game_event.h>

class GameObject;

class DeleteEvent : public GameEvent
{
public:
	DeleteEvent(int posX, int posY, GameObject * aGameObject);
	void DeleteEvent::Accept(GameEventVisitor* eventVisitor);

	int posX;
	int posY;

	GameObject * gameObject;
};