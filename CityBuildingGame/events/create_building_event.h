#pragma once
#include <event.h>

enum BuildingType { LumberjackHutID, DwellingID};

class CreateBuildingEvent : public Event
{
public:
	CreateBuildingEvent(BuildingType buildingType, float posX, float posY);
	
	void CreateBuildingEvent::Accept(EventVisitor* eventVisitor);
	
	BuildingType buildingType;

	float posX;
	float posY;
};