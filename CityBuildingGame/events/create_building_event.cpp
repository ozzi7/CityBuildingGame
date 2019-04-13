#include "stdafx.h"
#pragma once
#include "create_building_event.h"

CreateBuildingEvent::CreateBuildingEvent(BuildingType aBuildingType, float aPosX, float aPosY)
{
	posX = aPosX;
	posY = aPosY;
	buildingType = aBuildingType;
};

void CreateBuildingEvent::Accept(GameEventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
};