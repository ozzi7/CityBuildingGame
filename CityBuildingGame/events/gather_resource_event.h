#pragma once
#include "game_event.h"

class BoneAnimated;

class GatherResourceEvent : public GameEvent
{
public:
	GatherResourceEvent(Resource aResource, BoneAnimated* aPerson);

	void Accept(GameEventVisitor* eventVisitor) override;

	Resource resource;
	BoneAnimated* person;
};
