#include "stdafx.h"
#include "pathfinding_resource.h"

PathfindingResource::PathfindingResource(Grid* aGrid, const std::pair<int, int> XYstart)
{
	grid = aGrid;
}

// TODO, dummy code
void PathfindingResource::FindResource()
{
	PathfindingObject* pathFinding = new PathfindingObject(grid, std::make_pair(0,0));
	pathFinding->FindClosestIdleBuilding();
	std::list<std::pair<int,int>> pathCoordinatesList = pathFinding->GetPath();
	std::pair<int,int> coordinate = pathCoordinatesList.front();
	try
	{
		targetBuilding = dynamic_cast<Building*>(grid->gridUnits[coordinate.second][coordinate.first].objects.front());
		resourceBuilding = targetBuilding;
	} catch (const std::exception& e) {}

	pathFinding = new PathfindingObject(grid, std::make_pair(0,0));
	pathFinding->FindClosestIdleWorker();
	pathCoordinatesList = pathFinding->GetPath();
	coordinate = pathCoordinatesList.front();
	try
	{
		targetWorker = dynamic_cast<Worker*>(grid->gridUnits[coordinate.second][coordinate.first].objects.front());
	} catch (const std::exception& e) {}
}