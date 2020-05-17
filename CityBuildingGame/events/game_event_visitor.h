#pragma once

class CreateBuildingEvent;
class MoveEvent;
class DeleteEvent;
class GatherResourceEvent;
class ReturnHomeEvent;
class WorkerArrivedEvent;
class BringResourceEvent;

class GameEventVisitor
{
public:
	virtual void Visit(CreateBuildingEvent* aCreateBuildingEvent) = 0;
	virtual void Visit(MoveEvent* aMoveObject) = 0;
	virtual void Visit(DeleteEvent* aDeleteEvent) = 0;
	virtual void Visit(GatherResourceEvent* aGatherResourceEvent) = 0;
	virtual void Visit(ReturnHomeEvent* aReturnHomeEvent) = 0;
	virtual void Visit(BringResourceEvent* aBringResourceEvent) = 0;
	virtual void Visit(WorkerArrivedEvent* aWorkerArrivedEvent) = 0;
};
