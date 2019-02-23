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
		currUpperLeftX = (int)upperLeft.x;
		currUpperLeftY = (int)upperLeft.y;
		currLowerRightX = (int)lowerRight.x;
		currLowerRightY = (int)lowerRight.y;

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
