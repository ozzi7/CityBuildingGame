#pragma once
#include <glm/glm.hpp>
#include "game_object.h"
#include "visitor.h"
#include <math.h>
#include <vector>

class Building : public GameObject
{
public:
	Building(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation);

	/* the building covers [fromX, toX] x [fromY, toY] */
	int sizeX = 0;
	int sizeY = 0;

	int fromX = 0;
	int fromY = 0;
	int toX = 0;
	int toY = 0;

	int entranceX = 0;
	int entranceY = 0;

	/* 0 when created, 1 when done constructing ? what about during construction? used to change 3d model currently */
	unsigned short evolutionStage = 0;
	std::vector<glm::mat4> buildingOutlines;

	void CreateBuildingOutline();
	virtual void Evolve() = 0;

protected:
};
