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
	
	/* 0 when created, 1 when done constructing (used for 3d model selection) */
	unsigned short evolutionStage = 0;
	std::vector<glm::mat4> buildingOutlines;
	std::vector<glm::mat4> woodModels;

	void CreateBuildingOutline();
	virtual void Evolve() = 0;
	void AddWoodBuildingMaterial();
	void AddStoneBuildingMaterial();

	/*building requirements needed & delivered.. */
	int workersPresent = 0;
	int workersRequired = 0;
	int workersOnTheWay = 0;

	int woodStored = 0;
	int woodRequired = 0;
	int woodOnTheWay = 0;

	int stoneStored = 0;
	int stoneRequired = 0;
	int stoneOnTheWay = 0;

protected:

};
