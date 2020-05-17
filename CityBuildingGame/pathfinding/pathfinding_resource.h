#pragma once
#include "pathfinding_object.h"

class PathfindingResource
{
public:
	Worker* targetWorker = nullptr;
	Building* resourceBuilding = nullptr;
	Building* targetBuilding = nullptr;
	
	PathfindingResource(Grid* grid, std::pair<int,int> XYstart);
	void FindResourceFromWorker();
	void FindResourceFromBuilding();

private:
	Grid* grid;
	std::pair<int, int> startCoordinate;
	GameObject* resource = nullptr;
	GameObject* destination = nullptr;

	void FindWood();
	void FindStone();
};

