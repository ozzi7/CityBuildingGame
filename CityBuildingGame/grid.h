#pragma once

#include <vector>
#include <list>
#include <random>
#include <algorithm>

#include "terrain.h"
#include "fir.h"
#include "lumberjack.h"
#include "grass.h"

/* Represents a grid subunit */
class Unit
{
public:
	bool occupied = false;
	float averageHeight = 0;
	std::list<Object*> objects;
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
	float GetHeight(float posX, float posY);

	void UpdateVisibleList(glm::vec2 &upperLeft, glm::vec2 &upperRight, glm::vec2 &lowerLeft, glm::vec2 &lowerRight);

	std::vector<std::vector<Unit*>> gridUnits;

	/* Used for rendering */
	std::mutex visibleUnitsMutex;
	int maximumVisibleUnits = 600 * 500; // maximum number of units stored in visibleUnits0 and 1

	std::vector<Unit*> *visibleUnits0;
	std::vector<Unit*> *visibleUnits1;
	std::vector<Unit*> *visibleUnits2;
	int visibleUnitsSizeToRender = 0;
	int visibleUnitsRendering = 0;
	int visibleUnitsToRender = 0;
	int visibleUnitsToFill = 1;
	int currUpperLeftX = 0;
	int currUpperLeftY = 0;
	int currLowerRightX = 0;
	int currLowerRightY = 0;

	int gridHeight;
	int gridWidth;

	Terrain* terrain;
};
