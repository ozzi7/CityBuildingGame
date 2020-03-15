#include "stdafx.h"
#include "worker_arrived_event.h"

WorkerArrivedEvent::WorkerArrivedEvent(GameObject* aGameObject)
{
	gameObject = aGameObject;
}

void WorkerArrivedEvent::Accept(GameEventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
}
