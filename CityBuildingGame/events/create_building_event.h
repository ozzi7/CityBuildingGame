#pragma once
#include <game_event.h>

class CreateBuildingEvent : public GameEvent
{
public:
	CreateBuildingEvent(BuildingType buildingType, float posX, float posY);

	void CreateBuildingEvent::Accept(GameEventVisitor* eventVisitor) override;

	BuildingType buildingType;

	float posX;
	float posY;
};
