#pragma once

#include "grid.h"

Grid::Grid(int aGridHeight, int aGridWidth) {
	gridHeight = aGridHeight;
	gridWidth = aGridWidth;

	terrain = new Terrain(gridHeight, gridWidth);

	/* create grid including heights*/
	grid = new vector<vector<Unit*>>(gridHeight, vector<Unit*>(gridWidth, new Unit()));
	for (int i = 0; i < terrain->heightmap.size() - 1; ++i) {
		for (int j = 0; j < terrain->heightmap[i].size() - 1; ++j) {
			(*grid)[i][j]->averageHeight = (terrain->heightmap[i + 1][j] + terrain->heightmap[i][j + 1]) / 2.0f;
		}
	}

	/* create trees */
	NoiseGen noise_gen;
	vector<vector<float>> treeMap = vector<vector<float>>(gridHeight, vector<float>(gridWidth, 0));
	noise_gen.GeneratePerlinNoise(treeMap, gridHeight, gridWidth, 5, 6);

	for (int i = 0; i < gridHeight; ++i) {
		for (int j = 0; j < gridWidth; ++j) {
			if(treeMap[i][j] > 5.0f)
				(*grid)[i][j]->objects->push_back(new Tree(glm::vec3(0.0f,10.0f,10.0f)));
		}
	}
}
Grid::~Grid() {}

Unit::Unit() {}
Unit::~Unit() {}

