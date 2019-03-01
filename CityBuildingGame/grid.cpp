#include "stdafx.h"
#pragma once

#include "grid.h"

Grid::Grid(int aGridHeight, int aGridWidth) {
	gridHeight = aGridHeight;
	gridWidth = aGridWidth;

	terrain = new Terrain(gridHeight, gridWidth);
}

void Grid::Init()
{
	/* create gridUnits */
	for (int i = 0; i < gridHeight; ++i)
	{
		std::vector<Unit*> temp = std::vector<Unit*>();
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

	/*Initialize the vectors used for determining what to render*/
	visibleUnits = std::vector<Unit*>(maximumVisibleUnits);
}

void Grid::UpdateVisibleList(glm::vec2 &upperLeft, glm::vec2 &upperRight, glm::vec2 &lowerLeft, glm::vec2 &lowerRight)
{
	/* Check if we need to recalculate visible grid */
	if (currUpperLeftX != (int)upperLeft.x || currUpperLeftY != (int)upperLeft.y ||
		currLowerRightX != (int)lowerRight.x || currLowerRightY != (int)lowerRight.y)
	{
		currUpperLeftX = (int)upperLeft.x;
		currUpperLeftY = (int)upperLeft.y;
		currLowerRightX = (int)lowerRight.x;
		currLowerRightY = (int)lowerRight.y;

		int startX = std::min(std::min((int)upperLeft.x, int(lowerLeft.x)), std::min((int)upperRight.x, int(lowerRight.x)));
		int endX = std::max(std::max((int)upperLeft.x, int(lowerLeft.x)), std::max((int)upperRight.x, int(lowerRight.x)));
		int startY = std::min(std::min((int)upperLeft.y, int(lowerLeft.y)), std::min((int)upperRight.y, int(lowerRight.y)));
		int endY = std::max(std::max((int)upperLeft.y, int(lowerLeft.y)), std::max((int)upperRight.y, int(lowerRight.y)));

		int index = 0;
		for (int i = std::max(0, startY + 1); i <= std::min(gridHeight - 1, endY); ++i)
		{
			for (int j = std::max(0, startX + 1); j <= std::min(gridWidth - 1, endX); ++j)
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
						visibleUnits[index++] = gridUnits[i][j];
					}
				}
				else
					return;
			}
		}
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
	objects = std::list<Object*>();
	movingObjects = std::list<BoneAnimated*>();
}
Unit::~Unit() {
	for (auto it = objects.begin(); it != objects.end(); ++it)
		delete (*it);
	for (auto it = movingObjects.begin(); it != movingObjects.end(); ++it)
		delete (*it);
}
