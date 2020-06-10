#include "stdafx.h"
#pragma once
#include "create_building_preview_event.h"

CreateBuildingPreviewEvent::CreateBuildingPreviewEvent(BuildingType aBuildingType, float aPosX, float aPosY)
{
	posX = aPosX;
	posY = aPosY;
	buildingType = aBuildingType;
};

void CreateBuildingPreviewEvent::Accept(GameEventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
};
