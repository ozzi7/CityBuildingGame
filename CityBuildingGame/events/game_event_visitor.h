#pragma once

class CreateBuildingEvent;
class MoveEvent;
class DeleteEvent;
class GatherResourceEvent;

class GameEventVisitor
{
public:
	virtual void Visit(CreateBuildingEvent* aCreateBuildingEvent) = 0;
	virtual void Visit(MoveEvent * aMoveObject) = 0;
	virtual void Visit(DeleteEvent * aDeleteEvent) = 0;
	virtual void Visit(GatherResourceEvent * aGatherResourceEvent) = 0;
};