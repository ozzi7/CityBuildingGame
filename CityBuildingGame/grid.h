#pragma once

#include <vector>
#include <list>
#include <random>

#include "terrain.h"
#include "chamaecyparis.h"
#include "fir.h"

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

	vector<vector<Unit*>> gridunits;

	int gridHeight;
	int gridWidth;

	Terrain* terrain;
};
