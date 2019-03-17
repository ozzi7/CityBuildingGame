#pragma once
#include <glm/glm.hpp>
#include "game_object.h"
#include "visitor.h"
#include <math.h> 
#include <vector>
class Building : public GameObject
{
public:
	Building(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation, int aGridX, int aGridY);

	int sizeX = 0; // occupied units
	int sizeY = 0;

	/* 0 when built, 1 when built, then evolution, used to change 3d model currently */
	unsigned short growth = 0;
	std::vector<glm::mat4> buildingOutlines;

protected:
	void createBuildingOutline();
};