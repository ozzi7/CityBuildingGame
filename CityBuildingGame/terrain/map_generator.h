#pragma once

#include <random>
#include <globals.h>
#include "grid.h"
#include "oak.h"
#include "grass.h"
#include "toona.h"
#include "euro_beech.h"
#include "euro_beech_2.h"
#include "pine.h"
#include "lumberjack.h"
#include <cmath>
#include <pathfinding.h>
#include <pathfinding_object.h>
#include "bitmap_image.hpp"
#include <file_handler.h>

class MapGenerator
{
public:
	MapGenerator(Grid* aGrid);

	void GenerateMap();

private:
	Grid* grid;

	NoiseGen noiseGen;
	std::random_device rd;

	void generateTerrain() const;
	void generateTrees();
	void generateGrass();
	void flattenMap(std::vector<std::vector<float>>& pHeightmap) const;
	float getHeightAtPercentage(std::vector<std::vector<float>>& pHeightmap, float percentage) const;
	float getHeightAtPercentageExact(std::vector<std::vector<float>>& pHeightmap, float percentage) const;
	static float getGaussianPDFValue(float mean, float stddev, float x);
	static float getMinValue(std::vector<std::vector<float>>& pHeightmap);
	static float getMaxValue(std::vector<std::vector<float>>& pHeightmap);

	void SaveHeightmapImage(std::vector<std::vector<float>>& pHeightmap, std::string filename) const;
	void MapGenerator::SaveHeightmapToFile(std::vector<std::vector<float>>& pHeightmap, std::string filename) const;
	bool MapGenerator::LoadHeightmapFromFile(std::vector<std::vector<float>>& pHeightmap, std::string filename) const;
};
