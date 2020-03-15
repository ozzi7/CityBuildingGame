#pragma once
#include "pathfinding_object.h"

class PathfindingResource
{
public:
	void FindResource();
	Worker* targetWorker = nullptr;
	Building* resourceBuilding = nullptr;
	Building* targetBuilding = nullptr;
	PathfindingResource(Grid* grid, const std::pair<int,int> XYstart);

private:
	Grid* grid;
};

