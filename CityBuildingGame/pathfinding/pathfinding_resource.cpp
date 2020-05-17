#include "stdafx.h"
#include "pathfinding_resource.h"

PathfindingResource::PathfindingResource(Grid* aGrid, const std::pair<int, int> XYstart)
{
	grid = aGrid;
	startCoordinate = XYstart;
}


void PathfindingResource::FindResourceFromWorker()
{
	PathfindingObject* pathFindingWood = new PathfindingObject(grid, startCoordinate);
	pathFindingWood->FindClosestUnusedWood();

	if (pathFindingWood->GetDestinationObject() != nullptr)
	{
		PathfindingObject* pathFindingRequiredWood = new PathfindingObject(grid, std::pair<int, int>(pathFindingWood->GetDestinationObject()->posX,
																									 pathFindingWood->GetDestinationObject()->posY));
		pathFindingRequiredWood->FindClosestWoodRequired();

		if (pathFindingRequiredWood != nullptr)
		{
			resource = pathFindingWood->GetDestinationObject();
			destination = pathFindingRequiredWood->GetDestinationObject();
		}
		delete pathFindingRequiredWood;
	}
	delete pathFindingWood;
	
	if (resource != nullptr || destination != nullptr)
	{
		PathfindingObject* pathFindingStone = new PathfindingObject(grid, startCoordinate);
		pathFindingStone->FindClosestUnusedStone();

		if (pathFindingStone->GetDestinationObject() != nullptr)
		{
			PathfindingObject* pathFindingRequiredStone = new PathfindingObject(grid, std::pair<int, int>(pathFindingStone->GetDestinationObject()->posX,
																										  pathFindingStone->GetDestinationObject()->posY));
			pathFindingRequiredStone->FindClosestStoneRequired();

			if (pathFindingRequiredStone != nullptr)
			{
				resource = pathFindingStone->GetDestinationObject();
				destination = pathFindingRequiredStone->GetDestinationObject();
			}
			delete pathFindingRequiredStone;
		}
		delete pathFindingStone;
	}
	
	try
	{
		resourceBuilding = dynamic_cast<Building*>(resource);
	}
	catch (const std::exception& e) {} // Should not happen!

	try
	{
		targetBuilding = dynamic_cast<Building*>(destination);
	}
	catch (const std::exception& e) {} // Should not happen!
}

void PathfindingResource::FindResourceFromBuilding()
{
	PathfindingObject* pathFindingWood = new PathfindingObject(grid, startCoordinate);
	pathFindingWood->FindClosestUnusedWood();

	if (pathFindingWood->GetDestinationObject() != nullptr)
	{
		PathfindingObject* pathFindingWorker = new PathfindingObject(grid, std::pair<int, int>(pathFindingWood->GetDestinationObject()->posX,
																							   pathFindingWood->GetDestinationObject()->posY));
		pathFindingWorker->FindClosestIdleWorker();

		if (pathFindingWorker != nullptr)
		{
			resource = pathFindingWood->GetDestinationObject();
			destination = pathFindingWorker->GetDestinationObject();
		}
		delete pathFindingWorker;
	}
	delete pathFindingWood;
	
	if (resource != nullptr || destination != nullptr)
	{
		PathfindingObject* pathFindingStone = new PathfindingObject(grid, startCoordinate);
		pathFindingStone->FindClosestUnusedStone();

		if (pathFindingStone->GetDestinationObject() != nullptr)
		{
			PathfindingObject* pathFindingWorker = new PathfindingObject(grid, std::pair<int, int>(pathFindingStone->GetDestinationObject()->posX,
																								   pathFindingStone->GetDestinationObject()->posY));
			pathFindingWorker->FindClosestIdleWorker();

			if (pathFindingWorker != nullptr)
			{
				resource = pathFindingStone->GetDestinationObject();
				destination = pathFindingWorker->GetDestinationObject();
			}
			delete pathFindingWorker;
		}
		delete pathFindingStone;
	}
	
	try
	{
		resourceBuilding = dynamic_cast<Building*>(resource);
	}
	catch (const std::exception& e) {} // Should not happen!

	try
	{
		targetWorker = dynamic_cast<Worker*>(destination);
	}
	catch (const std::exception& e) {} // Should not happen!
}
