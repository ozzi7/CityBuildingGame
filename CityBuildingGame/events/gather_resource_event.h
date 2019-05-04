#pragma once
#include "game_event.h"

class BoneAnimated;

enum Resource { Wood };

class GatherResourceEvent : public GameEvent
{
public:
	GatherResourceEvent(Resource aResource, BoneAnimated* aPerson);

	void Accept(GameEventVisitor* eventVisitor);

	Resource resource;
	BoneAnimated* person;
};
