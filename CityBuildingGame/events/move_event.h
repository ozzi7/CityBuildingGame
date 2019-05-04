#pragma once
#include <game_event.h>

class GameObject;

class MoveEvent : public GameEvent
{
public:
	MoveEvent(int aFromX, int aFromY, int aToX, int aToY, GameObject* aGameObject);
	void MoveEvent::Accept(GameEventVisitor* eventVisitor);

	int fromX;
	int fromY;

	int toX;
	int toY;

	GameObject* gameObject;
};