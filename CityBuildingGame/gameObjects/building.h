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

	/* the building covers [startX, startX+sizeX-1] x [startY, startY+sizeY-1] */
	int sizeX = 0; // occupied units
	int sizeY = 0;
	int startX = 0;
	int startY = 0;

	/* 0 when created, 1 when done constructing ? what about during construction? used to change 3d model currently */
	unsigned short evolutionStage = 0;
	std::vector<glm::mat4> buildingOutlines;

protected:
	void createBuildingOutline();
};