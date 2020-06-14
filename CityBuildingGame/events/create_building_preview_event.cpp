#include "stdafx.h"
#pragma once
#include "create_building_preview_event.h"

CreateBuildingPreviewEvent::CreateBuildingPreviewEvent(BuildingType aBuildingType, float aPosX, float aPosY)
{
	StartPosition = std::make_pair(aPosX, aPosY);

	buildingType = aBuildingType;
};
CreateBuildingPreviewEvent::CreateBuildingPreviewEvent(BuildingType aBuildingType, float aPosXStart, float aPosYStart, float aPosXEnd, float aPosYEnd)
{
	StartPosition = std::make_pair(aPosXStart, aPosYStart);
	EndPosition = std::make_pair(aPosXEnd, aPosYEnd);

	buildingType = aBuildingType;
};
void CreateBuildingPreviewEvent::Accept(GameEventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
};
