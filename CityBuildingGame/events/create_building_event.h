#pragma once
#include <game_event.h>

class CreateBuildingEvent : public GameEvent
{
public:
	CreateBuildingEvent(BuildingType buildingType, float posX, float posY);
	CreateBuildingEvent(BuildingType aBuildingType, float aPosXStart, float aPosYStart, float aPosXEnd, float aPosYEnd);

	void CreateBuildingEvent::Accept(GameEventVisitor* eventVisitor) override;

	BuildingType buildingType;

	std::pair<int, int> StartPosition = std::make_pair(-1, -1);
	std::pair<int, int> EndPosition = std::make_pair(-1, -1);
};
