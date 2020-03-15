#pragma once
#include "game_event.h"

class GameObject;

class WorkerArrivedEvent : public GameEvent
{
public:
	WorkerArrivedEvent(GameObject* aGameObject);

	void Accept(GameEventVisitor* eventVisitor) override;

	GameObject* gameObject;
};
