#include "stdafx.h"
#include "pathfinding_resource.h"

PathfindingResource::PathfindingResource(Grid* aGrid, const std::pair<int, int> XYstart)
{
	grid = aGrid;
	startCoordinate = XYstart;
}


void PathfindingResource::FindResourceFromWorker()
{
	auto start = std::chrono::high_resolution_clock::now();
	
	PathfindingObject* pathFindingWood = new PathfindingObject(grid, startCoordinate);
	pathFindingWood->FindClosestUnusedWood();

	if (pathFindingWood->GetDestinationObject() != nullptr)
	{
		PathfindingObject* pathFindingRequiredWood = new PathfindingObject(grid, std::pair<int, int>(pathFindingWood->GetPath().back().first,
																									 pathFindingWood->GetPath().back().second));
		pathFindingRequiredWood->FindClosestWoodRequired();

		if (pathFindingRequiredWood->GetDestinationObject() != nullptr)
		{
			resource = pathFindingWood->GetDestinationObject();
			destination = pathFindingRequiredWood->GetDestinationObject();
		}
		delete pathFindingRequiredWood;
	}
	delete pathFindingWood;
	
	if (resource == nullptr || destination == nullptr)
	{
		PathfindingObject* pathFindingStone = new PathfindingObject(grid, startCoordinate);
		pathFindingStone->FindClosestUnusedStone();

		if (pathFindingStone->GetDestinationObject() != nullptr)
		{
			PathfindingObject* pathFindingRequiredStone = new PathfindingObject(grid, std::pair<int, int>(pathFindingStone->GetPath().back().first,
																										  pathFindingStone->GetPath().back().second));
			pathFindingRequiredStone->FindClosestStoneRequired();

			if (pathFindingRequiredStone->GetDestinationObject() != nullptr)
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

	auto elapsedTime = std::chrono::high_resolution_clock::now() - start;
	long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "Executed Pathfinding FindResourceFromWorker in " + std::to_string(elapsedTimeMicroseconds) + " microseconds"));
}

void PathfindingResource::FindResourceFromBuilding()
{
	auto start = std::chrono::high_resolution_clock::now();
	
	PathfindingObject* pathFindingWood = new PathfindingObject(grid, startCoordinate);
	pathFindingWood->FindClosestUnusedWood();

	if (pathFindingWood->GetDestinationObject() != nullptr)
	{
		PathfindingObject* pathFindingWorker = new PathfindingObject(grid, std::pair<int, int>(pathFindingWood->GetPath().back().first,
																							   pathFindingWood->GetPath().back().second));
		pathFindingWorker->FindClosestIdleWorker();

		if (pathFindingWorker->GetDestinationObject() != nullptr)
		{
			resource = pathFindingWood->GetDestinationObject();
			destination = pathFindingWorker->GetDestinationObject();
		}
		delete pathFindingWorker;
	}
	delete pathFindingWood;
	
	if (resource == nullptr || destination == nullptr)
	{
		PathfindingObject* pathFindingStone = new PathfindingObject(grid, startCoordinate);
		pathFindingStone->FindClosestUnusedStone();

		if (pathFindingStone->GetDestinationObject() != nullptr)
		{
			PathfindingObject* pathFindingWorker = new PathfindingObject(grid, std::pair<int, int>(pathFindingStone->GetPath().back().first,
																								   pathFindingStone->GetPath().back().second));
			pathFindingWorker->FindClosestIdleWorker();

			if (pathFindingWorker->GetDestinationObject() != nullptr)
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

	auto elapsedTime = std::chrono::high_resolution_clock::now() - start;
	long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "Executed Pathfinding FindResourceFromBuilding in " + std::to_string(elapsedTimeMicroseconds) + " microseconds"));
}
