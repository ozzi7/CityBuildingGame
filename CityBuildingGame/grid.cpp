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
/* returns true if the visible units changed */
bool Grid::UpdateVisibleList(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight)
{
	terrain->SetRenderWindow(upperLeft, upperRight, lowerLeft, lowerRight);

	/* Check if we need to recalculate visible grid */
	if (reloadGrid || currUpperLeftX != (int)upperLeft.x || currUpperLeftY != (int)upperLeft.y ||
		currLowerRightX != (int)lowerRight.x || currLowerRightY != (int)lowerRight.y)
	{
		reloadGrid = false;

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
					return true;
			}
		}
		return true;
	}
	return false;
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
Input for a 2x2 building is (0,1, 0,1) but accesses heightmap (0,2, 0,2)*/
bool Grid::IsAreaFlat(int fromX, int toX, int fromY, int toY) const
{
	if (fromX < 0 || toX >= gridWidth || fromY < 0 || toY >= gridHeight) // assumes toX > fromX etc.
		return false;

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
	if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
		return false;
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
void Grid::SetHasRoad(int x, int y, bool value)
{
	gridUnits[y][x].hasRoad = value;
}
void Grid::SetHasRoad(std::vector<std::pair<int,int>> road, bool value)
{
	for (std::pair<int, int> roadPiece : road)
	{
		SetHasRoad(roadPiece.first, roadPiece.second, value);
	}
}
/// <summary>
/// Checks if the x/y coordinates of a vector are inside the grid (after casting to int)
/// </summary>
/// <param name="position">a vector with x/y/z coordinates as floats or int</param>
bool Grid::IsValidPosition(glm::vec3 position) const
{
	return position.x >= 0 && position.y >= 0 &&
		gridHeight > (int)position.y &&
		gridWidth > (int)position.x;
}
void Grid::SetHasRoadPreview(int x, int y, bool value)
{
	gridUnits[y][x].hasRoadPreview = value;
}

bool Grid::IsValidBuildingPosition(int fromX, int fromY, int toX, int toY) const
{
	/* Check if the building is outside of the grid */
	if (fromX < 0 || toX >= gridWidth || fromY < 0 || toY >= gridHeight)
		return false;

	/* Check if the grid is not occupied */
	for (int i = fromX; i <= toX; ++i)
		for (int j = fromY; j <= toY; ++j)
			if (gridUnits[j][i].occupied || HasRoad(i, j))
				return false;

	/* Check if the floor is flat */
	if (!IsAreaFlat(fromX, toX, fromY, toY))
		return false;
	
	return true;
}
bool Grid::HasTree(int x, int y) const
{
	if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
		return false;

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
	if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
		return false;

	return gridUnits[y][x].hasRoad;
}
bool Grid::HasRoadPreview(int x, int y) const
{
	if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
		return false;

	return gridUnits[y][x].hasRoadPreview;
}
void Grid::ClearRoadPreview()
{
	for (std::pair<int, int> entry : roadCoordinates)
	{
		SetHasRoadPreview(entry.first, entry.second, false);
	}
	roadCoordinates.clear();
}
void Grid::SetHasRoadPreview(std::vector<std::pair<int, int>> road, bool value)
{
	roadCoordinates = std::vector<std::pair<int, int>>(road.begin(), road.end());
	for (std::pair<int, int> entry : roadCoordinates)
	{
		SetHasRoadPreview(entry.first, entry.second, value);
	}
}
/*
Returns true if any of the 4 surrounding tiles is a road
*/
bool Grid::HasRoadAccess(int x, int y) const
{
	if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
		return false;

	return HasRoad(x - 1, y) || HasRoad(x + 1, y)
		|| HasRoad(x, y - 1) || HasRoad(x, y + 1);
}
/*
Returns true if any of the 4 surrounding tiles is a road anywhere along the road
*/
bool Grid::HasRoadAccess(std::vector<std::pair<int, int>> road) const
{
	for (std::pair<int,int> roadPiece : road)
	{
		if (HasRoadAccess(roadPiece.first, roadPiece.second))
			return true;
	}
	return false;
}
/*
Returns true if any part of the road is on the border of the map
*/
bool Grid::IsAtEdgeOfMap(std::vector<std::pair<int, int>> road) const
{
	for (std::pair<int, int> roadPiece : road)
	{
		if ((roadPiece.first == 0 || roadPiece.first == gridWidth - 1 ||
			roadPiece.second == 0 || roadPiece.second == gridHeight - 1) 
			&& !IsOccupied(roadPiece.first, roadPiece.second))
			return true;
	}
	return false;
}

void Grid::DeleteGrass(int fromX, int fromY, int toX, int toY)
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
void Grid::DeleteGrass(std::vector<std::pair<int, int>> road)
{
	for (std::pair<int, int> roadPiece : road)
	{
		DeleteGrass(roadPiece.first, roadPiece.second, roadPiece.first, roadPiece.second);
	}
}
// returns -1,-1 for not found
std::pair<int, int> Grid::FindRoadAccess(int fromX, int toX, int fromY, int toY) const
{
	// top row
	if (toY + 1 >= 0 && toY + 1 <= gridHeight - 1)
	{
		for (int x = std::max(0, fromX); x <= std::min(gridWidth - 1, toX); ++x)
		{
			if (HasRoad(x, toY + 1))
				return std::make_pair(x, toY);
		}
	}
	// bottom row
	if (fromY - 1 >= 0 && fromY - 1 <= gridHeight - 1)
	{
		for (int x = std::max(0, fromX); x <= std::min(gridWidth - 1, toX); ++x)
		{
			if (HasRoad(x, fromY -1))
				return std::make_pair(x, fromY);
		}
	}
	// left column
	if (toX + 1 >= 0 && toX + 1 <= gridWidth - 1)
	{
		for (int y = std::max(0, fromY); y <= std::min(gridWidth - 1, toY); ++y)
		{
			if (HasRoad(toX + 1, y))
				return std::make_pair(toX, y);
		}
	}
	// right column
	if (fromX - 1 >= 0 && fromX - 1 <= gridWidth - 1)
	{
		for (int y = std::max(0, fromY); y <= std::min(gridWidth - 1, toY); ++y)
		{
			if (HasRoad(fromX - 1, y))
				return std::make_pair(fromX, y);
		}
	}

	return std::make_pair(-1,-1); // ??
}
Grid::~Grid()
{
	delete terrain;
}

Unit::Unit()
{}

Unit::~Unit()
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
		delete *it;
	for (auto it = movingObjects.begin(); it != movingObjects.end(); ++it)
		delete *it;
}
