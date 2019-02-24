#pragma once

#include <random>

#include "globals.h"
#include "grid.h"
#include "gameObjects/fir.h"
#include "gameObjects/palm.h"
#include "gameObjects/lumberjack.h"
#include "gameObjects/grass.h"

class MapGenerator {
public:
	MapGenerator(Grid* aGrid);

	void GenerateMap();

private:
	Grid* grid;

	NoiseGen noiseGen;
	std::random_device rd;
	vector<vector<float>> treeMap;

	void generateTerrain();
	void generateFirs();
};
