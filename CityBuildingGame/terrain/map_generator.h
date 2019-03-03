#pragma once

#include <random>

#include <globals.h>
#include "grid.h"
#include "oak.h"
#include "juniper.h"
#include "spruce.h"
#include "pine.h"
#include "lumberjack.h"
#include <cmath>

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
	void generateTrees();
	void flattenMap(std::vector<std::vector<float>> &pHeightmap);
	float getHeightAtPercentage(std::vector<std::vector<float>> &pHeightmap, float percentage);
	float getGaussianPDFValue(float mean, float stddev, float x);
};
