#pragma once
#include <game_event.h>

class CreateBuildingPreviewEvent : public GameEvent
{
public:
	CreateBuildingPreviewEvent(BuildingType buildingType, float posX, float posY);
	CreateBuildingPreviewEvent(BuildingType aBuildingType, float aPosXStart, float aPosYStart, float aPosXEnd, float aPosYEnd);

	void CreateBuildingPreviewEvent::Accept(GameEventVisitor* eventVisitor) override;

	BuildingType buildingType;

	std::pair<int,int> StartPosition = std::make_pair(-1, -1);
	std::pair<int, int> EndPosition = std::make_pair(-1, -1);
};
