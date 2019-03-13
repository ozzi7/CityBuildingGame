#pragma once
#include <event.h>

enum BuildingType { LumberjackHutID, DwellingID};

class CreateBuildingEvent : public Event
{
public:
	CreateBuildingEvent(BuildingType buildingType, int posX, int posY);
	
	void CreateBuildingEvent::Accept(EventVisitor* eventVisitor);
	
	BuildingType buildingType;

	int posX;
	int posY;
};