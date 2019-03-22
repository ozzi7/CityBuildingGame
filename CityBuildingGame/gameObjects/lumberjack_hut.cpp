#include "stdafx.h"
#pragma once
#include <lumberjack_hut.h>


LumberjackHut::LumberjackHut(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation, int aSizeX, int aSizeY)
	: Building(aPosition, aScale, aRotation, aSizeX, aSizeY) {

	sizeX = 2;
	sizeY = 2;
	startX = gridX - sizeX / 2;
	evolutionStage = 0;

};
void LumberjackHut::Accept(Visitor &v)
{
	v.Visit(this);
};