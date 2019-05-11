#pragma once

class CreateBuildingEvent;
class MoveEvent;
class DeleteEvent;
class GatherResourceEvent;
class ReturnHomeEvent;

class GameEventVisitor
{
public:
	virtual void Visit(CreateBuildingEvent* aCreateBuildingEvent) = 0;
	virtual void Visit(MoveEvent* aMoveObject) = 0;
	virtual void Visit(DeleteEvent* aDeleteEvent) = 0;
	virtual void Visit(GatherResourceEvent* aGatherResourceEvent) = 0;
	virtual void Visit(ReturnHomeEvent* aReturnHomeEvent) = 0;
};
