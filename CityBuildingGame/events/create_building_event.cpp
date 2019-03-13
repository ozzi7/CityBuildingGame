#include "stdafx.h"
#pragma once
#include "create_building_event.h"

CreateBuildingEvent::CreateBuildingEvent(BuildingType aBuildingType, int aPosX, int aPosY)
{
	posX = aPosX;
	posY = aPosY;
	buildingType = aBuildingType;
};

void CreateBuildingEvent::Accept(EventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
};