#include "stdafx.h"
#pragma once
#include "create_building_preview_event.h"

CreateBuildingPreviewEvent::CreateBuildingPreviewEvent(BuildingType aBuildingType, float aPosX, float aPosY)
{
	posX = aPosX;
	posY = aPosY;
	buildingType = aBuildingType;
};
CreateBuildingPreviewEvent::CreateBuildingPreviewEvent(BuildingType aBuildingType, float aPosXStart, float aPosYStart, float aPosXEnd, float aPosYEnd)
{
	posX = aPosXStart;
	posY = aPosYStart;
	posXEnd = aPosXEnd;
	posYEnd = aPosYEnd;
	buildingType = aBuildingType;
};
void CreateBuildingPreviewEvent::Accept(GameEventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
};
