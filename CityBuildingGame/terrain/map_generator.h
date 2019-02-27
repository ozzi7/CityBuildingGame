#pragma once

#include <random>

#include <globals.h>
#include "grid.h"
#include "fir.h"
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
	std::vector<std::vector<float>> treeMap;

	void generateTerrain();
	void generateFirs();
};
