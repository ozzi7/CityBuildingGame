#pragma once
#include <game_event.h>

class CreateBuildingPreviewEvent : public GameEvent
{
public:
	CreateBuildingPreviewEvent(BuildingType buildingType, float posX, float posY);

	void CreateBuildingPreviewEvent::Accept(GameEventVisitor* eventVisitor) override;

	BuildingType buildingType;

	float posX;
	float posY;
};
