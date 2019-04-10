#include "stdafx.h"
#include "gather_resource_event.h"


GatherResourceEvent::GatherResourceEvent(Resource aResource, BoneAnimated* aPerson)
{
	resource = aResource;
	person = aPerson;
}

void GatherResourceEvent::Accept(EventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
}

