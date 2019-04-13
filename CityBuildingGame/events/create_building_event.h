#pragma once
#include <game_event.h>

enum BuildingType { LumberjackHutID, DwellingID};

class CreateBuildingEvent : public GameEvent
{
public:
	CreateBuildingEvent(BuildingType buildingType, float posX, float posY);
	
	void CreateBuildingEvent::Accept(GameEventVisitor* eventVisitor);
	
	BuildingType buildingType;

	float posX;
	float posY;
};