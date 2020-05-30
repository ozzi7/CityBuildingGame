#pragma once

#include <vector>
#include <list>
#include <random>
#include <algorithm>

#include "terrain.h"
#include "oak.h"
#include "euro_beech.h"
#include "euro_beech_2.h"
#include "toona.h"
#include "pine.h"
#include "worker.h"
#include "building.h"

/* Represents a grid subunit */
class Unit
{
public:
	bool occupied = false;
	bool hasTree = false;
	float averageHeight = 0;
	std::list<GameObject*> objects;
	std::list<BoneAnimated*> movingObjects; // TODO doesnt have to coincide with bone animated.. hmm

	Unit();
	~Unit();
};

class Grid
{
public:
	Grid(int aGridHeight, int aGridWidth);
	~Grid();

	void Init();
	float GetHeight(float posX, float posY) const;
	bool IsAreaFlat(int fromX, int toX, int fromY, int toY) const;
	bool IsValidBuildingPosition(int fromX, int fromY, int toX, int toY) const;
	void UpdateVisibleList(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight);
	void SetIsOccupied(int fromX, int toX, int fromY, int toY, bool value);
	bool HasTree(int x, int y);
	bool IsOccupied(int x, int y);
	void SetHasTree(int x, int y, bool value);
	void SetIsOccupied(int x, int y, bool value);
	bool HasBuilding(int x, int y);
	bool HasBuilding(int fromX, int toX, int fromY, int toY);

	// [y][x]
	std::vector<std::vector<Unit>> gridUnits;

	std::vector<Unit*> visibleUnits;
	int nofVisibleUnits = 0;
	int maximumVisibleUnits = 600 * 500; // maximum number of units stored in visibleUnits
	int currUpperLeftX = 0;
	int currUpperLeftY = 0;
	int currLowerRightX = 0;
	int currLowerRightY = 0;

	int gridHeight;
	int gridWidth;

	bool buildingMode = false;

	Terrain* terrain;
};
