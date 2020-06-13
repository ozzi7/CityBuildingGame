#pragma once
#include <game_event.h>

class CreateBuildingEvent : public GameEvent
{
public:
	CreateBuildingEvent(BuildingType buildingType, float posX, float posY);
	CreateBuildingEvent(BuildingType aBuildingType, float aPosXStart, float aPosYStart, float aPosXEnd, float aPosYEnd);

	void CreateBuildingEvent::Accept(GameEventVisitor* eventVisitor) override;

	BuildingType buildingType;

	float posX;
	float posY;

	float posXEnd = -1;
	float posYEnd = -1;
};
