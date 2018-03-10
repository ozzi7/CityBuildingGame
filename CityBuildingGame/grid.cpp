#pragma once

#include "grid.h"

Grid::Grid(int aGridHeight, int aGridWidth) {
	gridHeight = aGridHeight;
	gridWidth = aGridWidth;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> pos_offset(-0.3, 0.3);
	std::uniform_real_distribution<> scale(0.7, 1.3);
	std::uniform_real_distribution<> rotation(0, 360.0f);

	terrain = new Terrain(gridHeight, gridWidth);

	/* create gridUnits */
	for (int i = 0; i < gridHeight; ++i)
	{
		vector<Unit*> temp = vector<Unit*>();
		for (int j = 0; j < gridWidth; ++j)
		{
			temp.push_back(new Unit());
		}
		gridUnits.push_back(temp);
	}

	/* calculate height of each grid unit */
	for (int i = 0; i < terrain->heightmap.size() - 1; ++i) {
		for (int j = 0; j < terrain->heightmap[i].size() - 1; ++j) {
			gridUnits[i][j]->averageHeight = (terrain->heightmap[i + 1][j] + terrain->heightmap[i][j + 1]) / 2.0f;
		}
	}

	/* create trees using noise */
	NoiseGen noise_gen;
	vector<vector<float>> treeMap = vector<vector<float>>(gridHeight, vector<float>(gridWidth, 0));
	noise_gen.GeneratePerlinNoise(treeMap, gridHeight, gridWidth, 10, 3);

	for (int i = 0; i < gridHeight; ++i) {
		for (int j = 0; j < gridWidth; ++j) {
			//if(treeMap[i][j] > 4.0f && treeMap[i][j] < 7.0f)
				//gridUnits[i][j]->objects.push_back(
					//new Chamaecyparis(glm::vec3(j + 0.5f + pos_offset(gen), i + 0.5f + pos_offset(gen), gridUnits[i][j]->averageHeight),
					/*new Palm(glm::vec3(j + 0.5f + pos_offset(gen), i + 0.5f + pos_offset(gen), gridUnits[i][j]->averageHeight),
						glm::vec3(scale(gen), scale(gen), scale(gen)),
						rotation(gen)));*/
			if (treeMap[i][j] >= 7.0f)
				gridUnits[i][j]->objects.push_back(
					new Fir(glm::vec3(j + 0.5f + pos_offset(gen), i + 0.5f + pos_offset(gen), gridUnits[i][j]->averageHeight),
						glm::vec3(scale(gen), scale(gen), scale(gen)),
						rotation(gen)));
		}
	}

	/*Initialize the vectors used for determining what to render*/
	visibleUnits0 = new vector<Unit*>(maximumVisibleUnits);
	visibleUnits1 = new vector<Unit*>(maximumVisibleUnits);
}
void Grid::UpdateVisibleList(glm::vec2 &upperLeft, glm::vec2 &upperRight, glm::vec2 &lowerLeft, glm::vec2 &lowerRight)
{
	/* Check if we need to recalculate visible grid */
	if (currUpperLeftX != (int)upperLeft.x || currUpperLeftY != (int)upperLeft.y ||
		currLowerRightX != (int)lowerRight.x || currLowerRightY != (int)lowerRight.y)
	{
		currUpperLeftX = upperLeft.x;
		currUpperLeftY = upperLeft.y;
		currLowerRightX = lowerRight.x;
		currLowerRightY = lowerRight.y;


		int startX = min(min((int)upperLeft.x, int(lowerLeft.x)), min((int)upperRight.x, int(lowerRight.x)));
		int endX = max(max((int)upperLeft.x, int(lowerLeft.x)), max((int)upperRight.x, int(lowerRight.x)));
		int startY = min(min((int)upperLeft.y, int(lowerLeft.y)), min((int)upperRight.y, int(lowerRight.y)));
		int endY = max(max((int)upperLeft.y, int(lowerLeft.y)), max((int)upperRight.y, int(lowerRight.y)));

		vector<Unit*> *visibleUnitsTemp;
		visibleUnitsMutex.lock();
		if (activeVisibleUnits)
			visibleUnitsTemp = visibleUnits0;
		else
			visibleUnitsTemp = visibleUnits1;
		visibleUnitsMutex.unlock();

		int index = 0;
		for (int i = max(0, startY + 1); i <= min(gridHeight - 1, endY); ++i)
		{
			for (int j = max(0, startX + 1); j <= min(gridWidth - 1, endX); ++j)
			{
				if (index < maximumVisibleUnits)
				{
					/* Check if the point is inside the rectangle*/
					glm::vec2 AM = glm::vec2(j - upperLeft.x, i - upperLeft.y);
					glm::vec2 AB = glm::vec2(upperRight - upperLeft);
					glm::vec2 AD = glm::vec2(lowerLeft - upperLeft);

					if ((0 <= glm::dot(AM, AB)) && (glm::dot(AM, AB) < glm::dot(AB, AB)) &&
						(glm::dot(AM, AD) < glm::dot(AD, AD)) && 0 <= glm::dot(AM, AD))
					{
						(*visibleUnitsTemp)[index++] = gridUnits[i][j];
					}
				}
			}
		}
		visibleUnitsMutex.lock();
		if (activeVisibleUnits == 1)
		{
			activeVisibleUnits = 0;
		}
		else
		{
			activeVisibleUnits = 1;
		}
		visibleUnitsSize = index;
		visibleUnitsMutex.unlock();
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
