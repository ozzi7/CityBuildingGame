#pragma once

#include "grid.h"

Grid::Grid(int aGridHeight, int aGridWidth) {
	gridHeight = aGridHeight;
	gridWidth = aGridWidth;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> pos_offset_tree(-0.45, 0.45);
	std::uniform_real_distribution<> pos_offset_grass(-0.5, 0.5);
	std::chi_squared_distribution<> scale_tree(3.0);
	std::uniform_real_distribution<> scale_tree_(0.03, 1.3f);
	std::uniform_real_distribution<> scale_grass(0.9, 1.2);
	std::uniform_real_distribution<> rotation(0, glm::two_pi<float>());

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
	noise_gen.GeneratePerlinNoise(treeMap, gridHeight, gridWidth, 0.0f, 10.0f, 3);

	/* TODO: this is temporary & ugly*/
	for (int i = 0; i < gridHeight; ++i) {
		for (int j = 0; j < gridWidth; ++j) {
			//if(treeMap[i][j] > 4.0f && treeMap[i][j] < 7.0f)
				//gridUnits[i][j]->objects.push_back(
					//new Chamaecyparis(glm::vec3(j + 0.5f + pos_offset(gen), i + 0.5f + pos_offset(gen), gridUnits[i][j]->averageHeight),
					/*new Palm(glm::vec3(j + 0.5f + pos_offset(gen), i + 0.5f + pos_offset(gen), gridUnits[i][j]->averageHeight),
						glm::vec3(scale(gen), scale(gen), scale(gen)),
						rotation(gen)));*/
			float scale = scale_tree(gen);
			while (scale < 0 || scale > 12) { scale = scale_tree(gen); }
			scale = 12 - scale;
			scale = scale / 10.0f;

			if (treeMap[i][j] >= 4.5f) {
				float posX = j + 0.5f + pos_offset_tree(gen);
				float posY = i + 0.5f + pos_offset_tree(gen);

				gridUnits[i][j]->objects.push_back(
					new Fir(glm::vec3(posX, posY, GetHeight(posX, posY)),
						glm::vec3(scale, scale, scale),
						rotation(gen)));
			}
			scale = scale_tree(gen);
			while (scale < 0 || scale > 12) { scale = scale_tree(gen); }
			scale = 12 - scale;
			scale = scale / 10.0f;

			if (treeMap[i][j] >= 7.0f) {
				float posX = j + 0.5f + pos_offset_tree(gen);
				float posY = i + 0.5f + pos_offset_tree(gen);

				gridUnits[i][j]->objects.push_back(
					new Fir(glm::vec3(posX, posY, GetHeight(posX, posY)),
						glm::vec3(scale, scale, scale),
						rotation(gen)));
			}
			scale = scale_tree(gen);
			while (scale < 0 || scale > 12) { scale = scale_tree(gen); }
			scale = 12 - scale;
			scale = scale / 10.0f;

			if (treeMap[i][j] >= 8.5f) {
				float posX = j + 0.5f + pos_offset_tree(gen);
				float posY = i + 0.5f + pos_offset_tree(gen);

				gridUnits[i][j]->objects.push_back(
					new Fir(glm::vec3(posX, posY, GetHeight(posX, posY)),
						glm::vec3(scale, scale, scale),
						rotation(gen)));
			}
			if (treeMap[i][j] < 2.0f)
				for (int z = 0; z < 3; ++z) {
					float posX = j + 0.5f + pos_offset_grass(gen);
					float posY = i + 0.5f + pos_offset_grass(gen);
					gridUnits[i][j]->objects.push_back(
						new Grass(glm::vec3(posX, posY, GetHeight(posX, posY)),
							glm::vec3(scale_grass(gen), scale_grass(gen), scale_grass(gen)),
							rotation(gen)));
				}
			//else if (treeMap[i][j] < 3.0f)
			//gridUnits[i][j]->movingObjects.push_back(
			//	new Lumberjack(glm::vec3(j + 0.5f + pos_offset_tree(gen), i + 0.5f + pos_offset_tree(gen), gridUnits[i][j]->averageHeight),
			//		glm::vec3(scale_tree(gen), scale_tree(gen), scale_tree(gen)),
			//		rotation(gen)));
		}
	}
	gridUnits[0][0]->movingObjects.push_back(
		new Lumberjack(glm::vec3(0 + 0.5f + pos_offset_tree(gen), 0 + 0.5f + pos_offset_tree(gen), gridUnits[0][0]->averageHeight),
			glm::vec3(scale_tree(gen), scale_tree(gen), scale_tree(gen)),
			rotation(gen)));

	/*Initialize the vectors used for determining what to render*/
	visibleUnits0 = new vector<Unit*>(maximumVisibleUnits);
	visibleUnits1 = new vector<Unit*>(maximumVisibleUnits);
	visibleUnits2 = new vector<Unit*>(maximumVisibleUnits);
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
		if (visibleUnitsToFill == 0)
			visibleUnitsTemp = visibleUnits0;
		else if (visibleUnitsToFill == 1)
			visibleUnitsTemp = visibleUnits1;
		else
			visibleUnitsTemp = visibleUnits2;
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
				else
					goto end;
			}
		}
	end:
		visibleUnitsMutex.lock();	
		visibleUnitsToRender = visibleUnitsToFill;
		if (visibleUnitsRendering == 0 && visibleUnitsToFill == 1)
		{
			visibleUnitsToFill = 2;
		}
		else if (visibleUnitsRendering == 0 && visibleUnitsToFill == 2)
		{
			visibleUnitsToFill = 1;
		}
		else if (visibleUnitsRendering == 1 && visibleUnitsToFill == 0)
		{
			visibleUnitsToFill = 2;
		}
		else if (visibleUnitsRendering == 1 && visibleUnitsToFill == 2)
		{
			visibleUnitsToFill = 0;
		}
		else if (visibleUnitsRendering == 2 && visibleUnitsToFill == 0)
		{
			visibleUnitsToFill = 1;
		}
		else if (visibleUnitsRendering == 2 && visibleUnitsToFill == 1)
		{
			visibleUnitsToFill = 0;
		}

		visibleUnitsSizeToRender = index;
		visibleUnitsMutex.unlock();
	}
}
/* Return height of any point on the grid */
float Grid::GetHeight(float posX, float posY)
{
	// top left i+1 j, bottom right i, j+1
	// bottom left i, j top right i+1, j+1
	int i = (int)posY;
	int j = (int)posX;
	float offsetX = fmod(posX, 1.0f);
	float offsetY = fmod(posY, 1.0f);

	if (offsetX + offsetY <= 1.0f) {
		// left triangle
		float m = terrain->heightmap[i][j + 1] - terrain->heightmap[i][j];
		float n = terrain->heightmap[i + 1][j] - terrain->heightmap[i][j];
		return (offsetX*m + offsetY*n) + terrain->heightmap[i][j];
	}
	else {
		float m = terrain->heightmap[i + 1][j] - terrain->heightmap[i + 1][j + 1];
		float n = terrain->heightmap[i][j + 1] - terrain->heightmap[i + 1][j + 1];
		return ((1-offsetX)*m + (1-offsetY) * n) + terrain->heightmap[i + 1][j + 1];;
	}
}
Grid::~Grid() {
	delete terrain;
}

Unit::Unit() {
	objects = list<Object*>();
}
Unit::~Unit() {
	for (auto it = objects.begin(); it != objects.end(); ++it)
		delete (*it);
}
