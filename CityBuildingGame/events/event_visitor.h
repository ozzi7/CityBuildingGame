#pragma once

class CreateBuildingEvent;
class MoveEvent;

class EventVisitor
{
public:
	virtual void Visit(CreateBuildingEvent* aCreateBuildingEvent) = 0;
	virtual void Visit(MoveEvent * aMoveObject) = 0;
};