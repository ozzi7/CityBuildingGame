#pragma once
#include "game_event.h"

class GameObject;

class ResourceArrivedEvent : public GameEvent
{
public:
	ResourceArrivedEvent(Resource aResource, GameObject* aTargetGameObject);

	void Accept(GameEventVisitor* eventVisitor) override;

	Resource resource;
	GameObject* targetGameObject;
};
