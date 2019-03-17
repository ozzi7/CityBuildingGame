#include "stdafx.h"
#pragma once
#include <dwelling.h>


Dwelling::Dwelling(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation, int aGridX, int aGridY)
	: Building(aPosition, aScale, aRotation, aGridX, aGridY) {

	sizeX = 2;
	sizeY = 2;
	growth = 0;

	Building::createBuildingOutline();
};
void Dwelling::Accept(Visitor &v)
{
	v.Visit(this);
};