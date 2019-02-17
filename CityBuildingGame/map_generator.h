#pragma once

#include <random>

#include "grid.h"
#include "chamaecyparis.h"
#include "fir.h"
#include "palm.h"
#include "lumberjack.h"
#include "grass.h"

class MapGenerator {
public:
	MapGenerator(Grid* aGrid);

	void GenerateMap();

private:
	Grid* grid;

	NoiseGen noiseGen;
	std::random_device rd;
	vector<vector<float>> treeMap;

	void generateFirs();
};
