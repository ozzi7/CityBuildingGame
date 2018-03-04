#pragma once

#include <vector>
#include <list>

#include "terrain.h"
#include "tree.h"

using namespace std;

/* Represents a grid subunit */
class Unit
{
public:
	bool occupied = false;
	float averageHeight = 0;
	list<Tree*> *objects;
	Unit();
	~Unit();
};

class Grid
{
public:
	Grid(int aGridHeight, int aGridWidth);
	~Grid();

	vector<vector<Unit*>> *grid;

	int gridHeight;
	int gridWidth;

	Terrain* terrain;
};