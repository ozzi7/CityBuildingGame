#pragma once

#include <vector>
#include <list>
#include <random>

#include "terrain.h"
#include "chamaecyparis.h"
#include "fir.h"
#include "palm.h"

using namespace std;

/* Represents a grid subunit */
class Unit
{
public:
	bool occupied = false;
	float averageHeight = 0;
	list<Tree*> objects;
	Unit();
	~Unit();
};

class Grid
{
public:
	Grid(int aGridHeight, int aGridWidth);
	~Grid();

	void UpdateVisibleList(glm::vec2 &upperLeft, glm::vec2 &upperRight, glm::vec2 &lowerLeft, glm::vec2 &lowerRight);

	vector<vector<Unit*>> gridUnits;

	/* Used for rendering */
	mutex visibleUnitsMutex;
	int maximumVisibleUnits = 100 * 100; // maximum number of units stored in visibleUnits0 and 1
	vector<Unit*> *visibleUnits0;
	vector<Unit*> *visibleUnits1;
	int visibleUnitsSize = 0;
	int currUpperLeftX = 0;
	int currUpperLeftY = 0;
	int currLowerRightX = 0;
	int currLowerRightY = 0;
	int activeVisibleUnits = 0;

	int gridHeight;
	int gridWidth;

	Terrain* terrain;
};
