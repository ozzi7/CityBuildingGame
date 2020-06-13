#include "stdafx.h"
#pragma once
#include "create_building_event.h"

CreateBuildingEvent::CreateBuildingEvent(BuildingType aBuildingType, float aPosX, float aPosY)
{
	posX = aPosX;
	posY = aPosY;

	buildingType = aBuildingType;
};
CreateBuildingEvent::CreateBuildingEvent(BuildingType aBuildingType, float aPosXStart, float aPosYStart, float aPosXEnd, float aPosYEnd)
{
	posX = aPosXStart;
	posY = aPosYStart;
	posXEnd = aPosXEnd;
	posYEnd = aPosYEnd;
	buildingType = aBuildingType;
};
void CreateBuildingEvent::Accept(GameEventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
};
