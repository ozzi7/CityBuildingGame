#include "stdafx.h"
#pragma once

#include "grid.h"

Grid::Grid(int aGridHeight, int aGridWidth)
{
	gridHeight = aGridHeight;
	gridWidth = aGridWidth;

	terrain = new Terrain(gridHeight, gridWidth, this);
}

void Grid::Init()
{
	/* create gridUnits */
	gridUnits = std::vector<std::vector<Unit>>(gridHeight, std::vector<Unit>(gridWidth, Unit()));

	/* calculate height of each grid unit */
	for (int i = 0; i < terrain->heightmap.size() - 1; ++i)
	{
		for (int j = 0; j < terrain->heightmap[i].size() - 1; ++j)
		{
			gridUnits[i][j].averageHeight = (terrain->heightmap[i + 1][j] + terrain->heightmap[i][j + 1]) / 2.0f;
		}
	}

	/*Initialize the vectors used for determining what to render*/
	visibleUnits = std::vector<Unit*>(maximumVisibleUnits);
}

void Grid::UpdateVisibleList(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight)
{
	/* Check if we need to recalculate visible grid */
	if (currUpperLeftX != (int)upperLeft.x || currUpperLeftY != (int)upperLeft.y ||
		currLowerRightX != (int)lowerRight.x || currLowerRightY != (int)lowerRight.y)
	{
		currUpperLeftX = (int)upperLeft.x;
		currUpperLeftY = (int)upperLeft.y;
		currLowerRightX = (int)lowerRight.x;
		currLowerRightY = (int)lowerRight.y;

		int fromX = std::min(std::min((int)upperLeft.x, int(lowerLeft.x)),
		                     std::min((int)upperRight.x, int(lowerRight.x)));
		int endX = std::max(std::max((int)upperLeft.x, int(lowerLeft.x)),
		                    std::max((int)upperRight.x, int(lowerRight.x)));
		int fromY = std::min(std::min((int)upperLeft.y, int(lowerLeft.y)),
		                     std::min((int)upperRight.y, int(lowerRight.y)));
		int endY = std::max(std::max((int)upperLeft.y, int(lowerLeft.y)),
		                    std::max((int)upperRight.y, int(lowerRight.y)));

		nofVisibleUnits = 0;
		for (int i = std::max(0, fromY + 1); i <= std::min(gridHeight - 1, endY); ++i)
		{
			for (int j = std::max(0, fromX + 1); j <= std::min(gridWidth - 1, endX); ++j)
			{
				if (nofVisibleUnits < maximumVisibleUnits)
				{
					/* Check if the point is inside the rectangle*/
					glm::vec2 AM = glm::vec2(j - upperLeft.x, i - upperLeft.y);
					glm::vec2 AB = glm::vec2(upperRight - upperLeft);
					glm::vec2 AD = glm::vec2(lowerLeft - upperLeft);

					if (0 <= dot(AM, AB) && dot(AM, AB) < dot(AB, AB) &&
						dot(AM, AD) < dot(AD, AD) && 0 <= dot(AM, AD))
					{
						visibleUnits[nofVisibleUnits++] = &gridUnits[i][j];
					}
				}
				else
					return;
			}
		}
	}
}
/* Return height of any point on the grid
Note: No out of bounds checking*/
float Grid::GetHeight(float posX, float posY) const
{
	// top left i+1 j, bottom right i, j+1
	// bottom left i, j top right i+1, j+1
	int i = (int)posY;
	int j = (int)posX;
	float offsetX = fmod(posX, 1.0f);
	float offsetY = fmod(posY, 1.0f);

	if (offsetX + offsetY <= 1.0f)
	{
		// left triangle
		float m = terrain->heightmap[i][j + 1] - terrain->heightmap[i][j];
		float n = terrain->heightmap[i + 1][j] - terrain->heightmap[i][j];
		return offsetX * m + offsetY * n + terrain->heightmap[i][j];
	}
	float m = terrain->heightmap[i + 1][j] - terrain->heightmap[i + 1][j + 1];
	float n = terrain->heightmap[i][j + 1] - terrain->heightmap[i + 1][j + 1];
	return (1 - offsetX) * m + (1 - offsetY) * n + terrain->heightmap[i + 1][j + 1];
}
/* Check if area is flat within a rectangle of the grid
Input for a 2x2 building is (0,1, 0,1) but accesses heightmap (0,2, 0,2)
Note: includes the points, no out of bounds checking*/
bool Grid::IsAreaFlat(int fromX, int toX, int fromY, int toY) const
{
	float height = terrain->heightmap[fromY][fromX];
	for (int y = fromY; y <= toY + 1; ++y)
	{
		for (int x = fromX; x <= toX + 1; ++x)
		{
			if (terrain->heightmap[y][x] != height)
			{
				return false;
			}
		}
	}
	return true;
}
bool Grid::IsOccupied(int x, int y) const
{
	return gridUnits[y][x].occupied;
}
void Grid::SetIsOccupied(int x, int y, bool value) 
{
	gridUnits[y][x].occupied = value;

	if (buildingMode)
		terrain->reloadTerrain = true;
}
void Grid::SetIsOccupied(int fromX, int toX, int fromY, int toY, bool value)
{
	for (int y = fromY; y <= toY; ++y)
	{
		for (int x = fromX; x <= toX; ++x)
		{
			gridUnits[y][x].occupied = value;
		}
	}
	if (buildingMode)
		terrain->reloadTerrain = true;
}
bool Grid::IsValidBuildingPosition(int fromX, int fromY, int toX, int toY) const
{
	/* Check if the building is outside of the grid */
	if (fromX < 0 || toX >= gridWidth || fromY < 0 || toY >= gridHeight)
		return false;

	/* Check if the grid is not occupied */
	for (int i = fromX; i <= toX; ++i)
		for (int j = fromY; j <= toY; ++j)
			if (gridUnits[j][i].occupied)
				return false;

	/* Check if the floor is flat */
	if (!IsAreaFlat(fromX, toX, fromY, toY))
		return false;
	
	return true;
}
bool Grid::HasTree(int x, int y) const
{
	return gridUnits[y][x].hasTree;
}
bool Grid::HasTree(int fromX, int toX, int fromY, int toY) const
{
	for (int y = std::max(0, fromY); y <= std::min(gridHeight-1, toY); ++y)
	{
		for (int x = std::max(0, fromX); x <= std::min(gridWidth-1, toX); ++x)
		{
			if (gridUnits[y][x].hasTree)
				return true;
		}
	}
	return false;
}
void Grid::SetHasTree(int x, int y, bool value)
{
	gridUnits[y][x].hasTree = value;
}
bool Grid::HasBuilding(int x, int y) const
{
	return !HasTree(x, y) && IsOccupied(x, y); // TODO:  add isBuildingFlag?
}
bool Grid::HasBuilding(int fromX, int toX, int fromY, int toY) const
{
	for (int y = fromY; y <= toY; ++y)
	{
		for (int x = fromX; x <= toX; ++x)
		{
			if (!HasTree(x, y) && IsOccupied(x, y))
				return true;
		}
	}
	return false;
}
bool Grid::HasRoad(int x, int y) const
{
	return gridUnits[y][x].hasRoad;
}
void Grid::DeleteGrass(int fromX, int toX, int fromY, int toY)
{
	// TODO: deletes ALL objects not just grass
	for (int x = fromX; x <= toX; ++x)
	{
		for (int y = fromY; y <= toY; ++y)
		{
			for (std::list<GameObject*>::iterator it = gridUnits[y][x].objects.begin();
				it != gridUnits[y][x].objects.end(); ++it)
			{
				delete* it;
			}
			gridUnits[y][x].objects.clear();
		}
	}
}
Grid::~Grid()
{
	delete terrain;
}

Unit::Unit()
{
	//objects = std::list<GameObject*>();
	//movingObjects = std::list<BoneAnimated*>();
}

Unit::~Unit()
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
		delete *it;
	for (auto it = movingObjects.begin(); it != movingObjects.end(); ++it)
		delete *it;
}
