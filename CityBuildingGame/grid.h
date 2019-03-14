#pragma once

#include <vector>
#include <list>
#include <random>
#include <algorithm>

#include "terrain.h"
#include "oak.h"
#include "spruce.h"
#include "juniper.h"
#include "pine.h"
#include "bone_animated.h"

/* Represents a grid subunit */
class Unit
{
public:
	bool occupied = false;
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
	float GetHeight(float posX, float posY);
	bool IsAreaFlat(int fromX, int toX, int fromY, int toY);
	void UpdateVisibleList(glm::vec2 &upperLeft, glm::vec2 &upperRight, glm::vec2 &lowerLeft, glm::vec2 &lowerRight);

	std::vector<std::vector<Unit*>> gridUnits;

	std::vector<Unit*> visibleUnits;
	int nofVisibleUnits = 0;
	int maximumVisibleUnits = 600 * 500; // maximum number of units stored in visibleUnits
	int currUpperLeftX = 0;
	int currUpperLeftY = 0;
	int currLowerRightX = 0;
	int currLowerRightY = 0;

	int gridHeight;
	int gridWidth;

	Terrain* terrain;
};
