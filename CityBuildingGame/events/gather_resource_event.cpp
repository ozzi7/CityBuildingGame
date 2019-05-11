#include "stdafx.h"
#include "gather_resource_event.h"

GatherResourceEvent::GatherResourceEvent(Resource aResource, BoneAnimated* aPerson)
{
	resource = aResource;
	person = aPerson;
}

void GatherResourceEvent::Accept(GameEventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
}
