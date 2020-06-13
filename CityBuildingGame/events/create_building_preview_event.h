#pragma once
#include <game_event.h>

class CreateBuildingPreviewEvent : public GameEvent
{
public:
	CreateBuildingPreviewEvent(BuildingType buildingType, float posX, float posY);
	CreateBuildingPreviewEvent(BuildingType aBuildingType, float aPosXStart, float aPosYStart, float aPosXEnd, float aPosYEnd);

	void CreateBuildingPreviewEvent::Accept(GameEventVisitor* eventVisitor) override;

	BuildingType buildingType;

	float posX = -1;
	float posY = -1;

	float posXEnd = -1;
	float posYEnd = -1;
};
