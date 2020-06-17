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
		for (int j = 0; j < terrain->heightmap[i].size() - 1; ++j)
			gridUnits[i][j].averageHeight = (terrain->heightmap[i + 1][j] + terrain->heightmap[i][j + 1]) / 2.0f;

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

		const int fromX = std::min(std::min((int)upperLeft.x, int(lowerLeft.x)),
		                           std::min((int)upperRight.x, int(lowerRight.x)));
		const int endX = std::max(std::max((int)upperLeft.x, int(lowerLeft.x)),
		                          std::max((int)upperRight.x, int(lowerRight.x)));
		const int fromY = std::min(std::min((int)upperLeft.y, int(lowerLeft.y)),
		                           std::min((int)upperRight.y, int(lowerRight.y)));
		const int endY = std::max(std::max((int)upperLeft.y, int(lowerLeft.y)),
		                          std::max((int)upperRight.y, int(lowerRight.y)));

		nofVisibleUnits = 0;
		for (int i = std::max(0, fromY + 1); i <= std::min(gridHeight - 1, endY); ++i)
			for (int j = std::max(0, fromX + 1); j <= std::min(gridWidth - 1, endX); ++j)
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
	const int i = (int)posY;
	const int j = (int)posX;
	const float offsetX = fmod(posX, 1.0f);
	const float offsetY = fmod(posY, 1.0f);

	if (offsetX + offsetY <= 1.0f)
	{
		// left triangle
		const float m = terrain->heightmap[i][j + 1] - terrain->heightmap[i][j];
		const float n = terrain->heightmap[i + 1][j] - terrain->heightmap[i][j];
		return offsetX * m + offsetY * n + terrain->heightmap[i][j];
	}
	const float m = terrain->heightmap[i + 1][j] - terrain->heightmap[i + 1][j + 1];
	const float n = terrain->heightmap[i][j + 1] - terrain->heightmap[i + 1][j + 1];
	return (1 - offsetX) * m + (1 - offsetY) * n + terrain->heightmap[i + 1][j + 1];
}
/* Check if area is flat within a rectangle of the grid
Input for a 2x2 building is (0,1, 0,1) but accesses heightmap (0,2, 0,2)*/
bool Grid::IsAreaFlat(int fromX, int toX, int fromY, int toY) const
{
	if (fromX < 0 || toX >= gridWidth || fromY < 0 || toY >= gridHeight) // assumes toX > fromX etc.
		return false;

	const float height = terrain->heightmap[fromY][fromX];
	for (int y = fromY; y <= toY + 1; ++y)
		for (int x = fromX; x <= toX + 1; ++x)
			if (terrain->heightmap[y][x] != height)
				return false;
	
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
		for (int x = fromX; x <= toX; ++x)
			gridUnits[y][x].occupied = value;
	
	if (buildingMode)
		terrain->reloadTerrain = true;
}
void Grid::SetHasRoad(int x, int y, bool value)
{
	gridUnits[y][x].hasRoad = value;
	terrain->reloadTerrain = true;
}
void Grid::SetHasRoad(const std::vector<std::pair<int,int>>& road, bool value)
{
	for (const std::pair<int, int> roadPiece : road)
		gridUnits[roadPiece.second][roadPiece.first].hasRoad = value;
	terrain->reloadTerrain = true;
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
/// <summary>
/// Returns the closest valid coordinate
/// </summary>
/// <param name="coordinates">pair of x/y coordinates</param>
/// <returns></returns>
std::pair<int,int> Grid::GetClosestValidPosition(std::pair<int,int> coordinates) const
{
	return std::make_pair(std::max(0, std::min(gridWidth - 1, coordinates.first)),
		std::max(0, std::min(gridHeight - 1, coordinates.second)));
}
/// <summary>
/// Returns the closest valid coordinate
/// </summary>
/// <param name="coordinates">pair of x/y coordinates</param>
/// <returns></returns>
std::pair<int, int> Grid::GetClosestValidPosition(std::pair<float, float> coordinates) const
{
	std::pair<int, int> closestIntCoords = std::make_pair(round(coordinates.first), round(coordinates.second));
	return std::make_pair(std::max(0, std::min(gridWidth - 1, closestIntCoords.first)),
		std::max(0, std::min(gridHeight - 1, closestIntCoords.second)));
}
void Grid::SetHasRoadPreview(int x, int y, bool value)
{
	ClearRoadPreview();

	roadCoordinates.push_back(std::make_pair(x, y));
	gridUnits[y][x].hasRoadPreview = value;
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
		for (int x = std::max(0, fromX); x <= std::min(gridWidth-1, toX); ++x)
			if (gridUnits[y][x].hasTree)
				return true;
	
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
		for (int x = fromX; x <= toX; ++x)
			if (!HasTree(x, y) && IsOccupied(x, y))
				return true;
	
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
	for (const std::pair<int, int> roadPiece : roadCoordinates)
		gridUnits[roadPiece.second][roadPiece.first].hasRoadPreview = false;
	
	roadCoordinates.clear();
	terrain->reloadTerrain = true;
}
void Grid::SetHasRoadPreview(std::vector<std::pair<int, int>> road, bool value)
{
	ClearRoadPreview();
	
	roadCoordinates = std::vector<std::pair<int, int>>(road.begin(), road.end());
	for (const std::pair<int, int> roadPiece : roadCoordinates)
		gridUnits[roadPiece.second][roadPiece.first].hasRoadPreview = value;
	terrain->reloadTerrain = true;
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
bool Grid::HasRoadAccess(const std::vector<std::pair<int, int>>& road) const
{
	for (const std::pair<int,int> roadPiece : road)
		if (HasRoadAccess(roadPiece.first, roadPiece.second))
			return true;
	
	return false;
}
/*
Returns true if any part of the road is on the border of the map
*/
bool Grid::IsAtEdgeOfMap(const std::vector<std::pair<int, int>>& road) const
{
	for (const std::pair<int, int> roadPiece : road)
		if ((roadPiece.first == 0 || roadPiece.first == gridWidth - 1 ||
			 roadPiece.second == 0 || roadPiece.second == gridHeight - 1))
		{
			return true;
		}
	
	return false;
}
bool Grid::IsAtEdgeOfMap(int x, int y) const
{
	return ((x == 0 || x == gridWidth - 1 ||
		y == 0 || y == gridHeight - 1));
}
void Grid::DeleteGrass(int fromX, int fromY, int toX, int toY)
{
	// TODO: deletes ALL objects not just grass
	for (int x = fromX; x <= toX; ++x)
		for (int y = fromY; y <= toY; ++y)
		{
			for (auto& object : gridUnits[y][x].objects)
				delete object;
			
			gridUnits[y][x].objects.clear();
		}
}
void Grid::DeleteGrass(const std::vector<std::pair<int, int>>& road)
{
	for (const std::pair<int, int> roadPiece : road)
		DeleteGrass(roadPiece.first, roadPiece.second, roadPiece.first, roadPiece.second);
	reloadGrid = true;
}
// returns -1,-1 if not found
std::pair<int, int> Grid::FindRoadAccess(int fromX, int toX, int fromY, int toY) const
{
	// top row
	if (toY + 1 > 0 && toY + 1 < gridHeight)
		for (int x = std::max(0, fromX); x <= std::min(gridWidth - 1, toX); ++x)
			if (HasRoad(x, toY + 1))
				return std::make_pair(x, toY);
	
	// bottom row
	if (fromY - 1 >= 0 && fromY - 1 < gridHeight)
		for (int x = std::max(0, fromX); x <= std::min(gridWidth - 1, toX); ++x)
			if (HasRoad(x, fromY -1))
				return std::make_pair(x, fromY);
	
	// left column
	if (toX + 1 >= 0 && toX + 1 < gridWidth)
		for (int y = std::max(0, fromY); y <= std::min(gridHeight - 1, toY); ++y)
			if (HasRoad(toX + 1, y))
				return std::make_pair(toX, y);
	
	// right column
	if (fromX - 1 >= 0 && fromX - 1 < gridWidth)
		for (int y = std::max(0, fromY); y <= std::min(gridHeight - 1, toY); ++y)
			if (HasRoad(fromX - 1, y))
				return std::make_pair(fromX, y);
	

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
	for (GameObject* object : objects)
		delete object;
	for (BoneAnimated* movingObject : movingObjects)
		delete movingObject;
}
