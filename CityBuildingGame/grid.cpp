#pragma once

#include "grid.h"

Grid::Grid(int aGridHeight, int aGridWidth) {
	gridHeight = aGridHeight;
	gridWidth = aGridWidth;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> pos_offset(-0.2, 0.2);
	std::uniform_real_distribution<> scale(0.5, 2.0);

	terrain = new Terrain(gridHeight, gridWidth);

	/* create gridunits */
	for (int i = 0; i < gridHeight; ++i)
	{
		vector<Unit*> temp = vector<Unit*>();
		for (int j = 0; j < gridWidth; ++j)
		{
			temp.push_back(new Unit());
		}
		gridunits.push_back(temp);
	}

	/* calculate height of each grid unit */
	for (int i = 0; i < terrain->heightmap.size() - 1; ++i) {
		for (int j = 0; j < terrain->heightmap[i].size() - 1; ++j) {
			gridunits[i][j]->averageHeight = (terrain->heightmap[i + 1][j] + terrain->heightmap[i][j + 1]) / 2.0f;
		}
	}

	/* create trees using noise */
	NoiseGen noise_gen;
	vector<vector<float>> treeMap = vector<vector<float>>(gridHeight, vector<float>(gridWidth, 0));
	noise_gen.GeneratePerlinNoise(treeMap, gridHeight, gridWidth, 10, 3);

	for (int i = 0; i < gridHeight; ++i) {
		for (int j = 0; j < gridWidth; ++j) {
			if(treeMap[i][j] > 4.0f && treeMap[i][j] < 7.0f)
				gridunits[i][j]->objects.push_back(
					new WhiteTree(glm::vec3(j + 0.5f + pos_offset(gen), i + 0.5f + pos_offset(gen), gridunits[i][j]->averageHeight)));
			else if (treeMap[i][j] >= 7.0f)
				gridunits[i][j]->objects.push_back(
					new Fir(glm::vec3(j + 0.5f + pos_offset(gen), i + 0.5f + pos_offset(gen), gridunits[i][j]->averageHeight)));
		}
	}
}
Grid::~Grid() {
	delete terrain;
}

Unit::Unit() {
	objects = list<Tree*>();
}
Unit::~Unit() {
	for (auto it = objects.begin(); it != objects.end(); ++it)
		delete (*it);
}

