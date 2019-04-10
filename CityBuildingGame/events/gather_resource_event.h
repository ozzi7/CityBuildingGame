#pragma once
#include "event.h"

class BoneAnimated;

enum Resource {Wood};

class GatherResourceEvent : public Event
{
public:
	GatherResourceEvent(Resource aResource, BoneAnimated* aPerson);

	void Accept(EventVisitor* eventVisitor);

	Resource resource;
	BoneAnimated* person;
};

