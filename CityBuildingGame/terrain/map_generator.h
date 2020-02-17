#pragma once

#include <random>
#include <globals.h>
#include "grid.h"
#include "oak.h"
#include "toona.h"
#include "euro_beech.h"
#include "euro_beech_2.h"
#include "pine.h"
#include "lumberjack.h"
#include <cmath>
#include <pathfinding.h>
#include <pathfinding_object.h>

class MapGenerator
{
public:
	MapGenerator(Grid* aGrid);

	void GenerateMap();

private:
	Grid* grid;

	NoiseGen noiseGen;
	std::random_device rd;
	std::vector<std::vector<float>> treeMap;

	void generateTerrain() const;
	void generateTrees();
	void flattenMap(std::vector<std::vector<float>>& pHeightmap) const;
	float getHeightAtPercentage(std::vector<std::vector<float>>& pHeightmap, float percentage) const;
	float getGaussianPDFValue(float mean, float stddev, float x);
};
