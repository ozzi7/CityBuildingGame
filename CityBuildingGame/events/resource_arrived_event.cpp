#include "stdafx.h"
#include "resource_arrived_event.h"

ResourceArrivedEvent::ResourceArrivedEvent(Resource aResource, GameObject* aTargetGameObject)
{
	resource = aResource;
	targetGameObject = aTargetGameObject;
}

void ResourceArrivedEvent::Accept(GameEventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
}
