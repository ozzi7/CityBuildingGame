#pragma once

#include <vector>
#include <list>
#include <random>
#include <algorithm>

#include "terrain.h"
#include "oak.h"
#include "euro_beech.h"
#include "euro_beech_2.h"
#include "toona.h"
#include "pine.h"
#include "worker.h"
#include "building.h"

/* Represents a grid subunit */
class Unit
{
public:
	bool occupied = false;
	bool hasTree = false;
	bool hasRoad = false;
	bool hasRoadPreview = false;
	float averageHeight = 0;
	std::list<GameObject*> objects;
	std::list<BoneAnimated*> movingObjects; // TODO doesnt have to coincide with bone animated.. hmm

	Unit();
	~Unit();
};

class Grid
{
public:
	Grid(int aGridHeight, int aGridWidth);
	~Grid();

	void Init();
	float GetHeight(float posX, float posY) const;
	bool IsAreaFlat(int fromX, int toX, int fromY, int toY) const;
	bool IsValidBuildingPosition(int fromX, int fromY, int toX, int toY) const;
	bool UpdateVisibleList(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight);
	void SetIsOccupied(int fromX, int toX, int fromY, int toY, bool value);
	void SetHasRoad(int x, int y, bool value);
	void SetHasRoad(std::vector<std::pair<int, int>> road, bool value);
	bool IsValidPosition(glm::vec3 position) const;
	void SetHasRoadPreview(int x, int y, bool value);
	void ClearRoadPreview();
	bool HasTree(int x, int y) const;
	bool HasTree(int fromX, int toX, int fromY, int toY) const;
	bool IsOccupied(int x, int y) const;
	void SetHasTree(int x, int y, bool value);
	void SetIsOccupied(int x, int y, bool value);
	bool HasBuilding(int x, int y) const;
	bool HasBuilding(int fromX, int toX, int fromY, int toY) const;
	bool HasRoad(int x, int y) const;
	bool HasRoadPreview(int x, int y) const;
	bool HasRoadAccess(int x, int y) const;
	bool HasRoadAccess(std::vector<std::pair<int, int>> road) const;
	bool IsAtEdgeOfMap(std::vector<std::pair<int, int>> road) const;
	void DeleteGrass(int fromX, int toX, int fromY, int toY);
	void DeleteGrass(std::vector<std::pair<int, int>> road);

	std::pair<int, int> FindRoadAccess(int fromX, int toX, int fromY, int toY) const;

	// [y][x]
	std::vector<std::vector<Unit>> gridUnits;

	std::vector<Unit*> visibleUnits;
	int nofVisibleUnits = 0;
	int maximumVisibleUnits = 600 * 500; // maximum number of units stored in visibleUnits
	int currUpperLeftX = 0;
	int currUpperLeftY = 0;
	int currLowerRightX = 0;
	int currLowerRightY = 0;

	int gridHeight;
	int gridWidth;

	std::list<GameObject*> previewObjects = std::list<GameObject *>();
	std::vector<std::pair<int, int>> roadCoordinates = std::vector<std::pair<int,int>>();
	bool buildingMode = false;

	Terrain* terrain;
};
