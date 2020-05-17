#include "stdafx.h"
#include "bring_resource_event.h"

BringResourceEvent::BringResourceEvent(Worker* aWorker, int aPrevWorkerState)
{
	worker = aWorker;
	prevWorkerState = aPrevWorkerState;
}

void BringResourceEvent::Accept(GameEventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
}
