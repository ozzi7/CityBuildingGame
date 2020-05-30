#include "stdafx.h"
#include "pathfinding_object.h"

PathfindingObject::PathfindingObject(Grid* aGrid, const std::pair<int,int> XYstart)
{
	grid = aGrid;

	start = new NodeObject();
	start->coordinate = XYstart;
	start->distanceToStart = 0;
	visited[XYstart.first][XYstart.second] = true;
}

PathfindingObject::~PathfindingObject()
{
	for (NodeObject* node : closed)
		delete node;

	while (!open.empty())
	{
		current = open.top();
		open.pop();
		delete current;
	}
}

void PathfindingObject::FindClosestTree()
{
	auto startTime = std::chrono::high_resolution_clock::now();
	
	objectType = ObjectType::tree;
	calculatePath();

	auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "Executed Pathfinding FindClosestTree from " +
																		 std::to_string(start->coordinate.first) + "|" +
																		 std::to_string(start->coordinate.second) + 
																		 " in " + std::to_string(elapsedTimeMicroseconds) + " microseconds"));
}

void PathfindingObject::FindClosestEdge()
{
	auto startTime = std::chrono::high_resolution_clock::now();
	
	objectType = ObjectType::edge;
	calculatePath();

	auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "Executed Pathfinding FindClosestEdge from " +
																		 std::to_string(start->coordinate.first) + "|" +
																		 std::to_string(start->coordinate.second) + 
																		 " in " + std::to_string(elapsedTimeMicroseconds) + " microseconds"));
}

void PathfindingObject::FindClosestIdleWorker()
{
	auto startTime = std::chrono::high_resolution_clock::now();
	
	objectType = ObjectType::idleWorker;
	calculatePath();

	auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "Executed Pathfinding FindClosestIdleWorker from " +
																		 std::to_string(start->coordinate.first) + "|" +
																		 std::to_string(start->coordinate.second) + 
																		 " in " + std::to_string(elapsedTimeMicroseconds) + " microseconds"));
}

void PathfindingObject::FindClosestIdleBuilding()
{
	auto startTime = std::chrono::high_resolution_clock::now();
	
	objectType = ObjectType::idleBuilding;
	calculatePath();

	auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "Executed Pathfinding FindClosestIdleBuilding from " +
																		 std::to_string(start->coordinate.first) + "|" +
																		 std::to_string(start->coordinate.second) + 
																		 " in " + std::to_string(elapsedTimeMicroseconds) + " microseconds"));
}

void PathfindingObject::FindClosestUnusedWood()
{
	auto startTime = std::chrono::high_resolution_clock::now();
	
	objectType = ObjectType::unusedWood;
	calculatePath();

	auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "Executed Pathfinding FindClosestUnusedWood from " +
																		 std::to_string(start->coordinate.first) + "|" +
																		 std::to_string(start->coordinate.second) + 
																		 " in " + std::to_string(elapsedTimeMicroseconds) + " microseconds"));
}

void PathfindingObject::FindClosestUnusedStone()
{
	auto startTime = std::chrono::high_resolution_clock::now();
	
	objectType = ObjectType::unusedStone;
	calculatePath();

	auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "Executed Pathfinding FindClosestUnusedStone from " +
																		 std::to_string(start->coordinate.first) + "|" +
																		 std::to_string(start->coordinate.second) + 
																		 " in " + std::to_string(elapsedTimeMicroseconds) + " microseconds"));
}

void PathfindingObject::FindClosestWoodRequired()
{
	auto startTime = std::chrono::high_resolution_clock::now();
	
	objectType = ObjectType::woodRequired;
	calculatePath();

	auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "Executed Pathfinding FindClosestWoodRequired from " +
																		 std::to_string(start->coordinate.first) + "|" +
																		 std::to_string(start->coordinate.second) + 
																		 " in " + std::to_string(elapsedTimeMicroseconds) + " microseconds"));
}

void PathfindingObject::FindClosestStoneRequired()
{
	auto startTime = std::chrono::high_resolution_clock::now();
	
	objectType = ObjectType::stoneRequired;
	calculatePath();

	auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "Executed Pathfinding FindClosestStoneRequired from " +
																		 std::to_string(start->coordinate.first) + "|" +
																		 std::to_string(start->coordinate.second) + 
																		 " in " + std::to_string(elapsedTimeMicroseconds) + " microseconds"));
}

std::list<std::pair<int,int>> PathfindingObject::GetPath() const
{
	std::list<std::pair<int,int>> path;
	NodeObject* currentPath = current;
	if (objectFound)
	{
		path.push_front(currentPath->coordinate);
		while (currentPath->parent != nullptr)
		{
			currentPath = currentPath->parent;
			path.push_front(currentPath->coordinate);
		}
	}
	return path;
}

GameObject* PathfindingObject::GetDestinationObject() const
{
	if (objectFound)
	{
		switch (objectType)
		{
			case ObjectType::tree:
				for (GameObject* object : grid->gridUnits[destination->coordinate.second][destination->coordinate.first].objects)
				{
					try
					{
						Tree* tree = dynamic_cast<Tree*>(object);
						if (tree)
							return object;
					}
					catch (const std::exception& e) {} // Not an exception, expected behavior...
				}
				break;

			case ObjectType::idleWorker:
				for (BoneAnimated* object : grid->gridUnits[destination->coordinate.second][destination->coordinate.first].movingObjects)
				{
					try
					{
						Worker* worker = dynamic_cast<Worker*>(object);
						if (worker)
							if (worker->state == State::idle)
								return object;
					}
					catch (const std::exception& e) {} // Not an exception, expected behavior...
				}
				break;

			case ObjectType::idleBuilding:
				return findBuildingReference(destination->coordinate);

			case ObjectType::unusedWood:
				return findBuildingReference(destination->coordinate);

			case ObjectType::unusedStone:
				return findBuildingReference(destination->coordinate);

			case ObjectType::woodRequired:
				return findBuildingReference(destination->coordinate);

			case ObjectType::stoneRequired:
				return findBuildingReference(destination->coordinate);

			default:
				return nullptr;
		}
	}
	return nullptr;
}

void PathfindingObject::calculatePath()
{
	open.push(start);
	checkObjectFound(start->coordinate);
	setNextNode();
	while (!objectFound && !unreachable)
	{
		if (current->coordinate.first < maxX)
			createNode(std::pair<int,int>(current->coordinate.first + 1, current->coordinate.second));
		if (current->coordinate.second < maxY && !objectFound)
			createNode(std::pair<int,int>(current->coordinate.first, current->coordinate.second + 1));
		if (current->coordinate.first > 0 && !objectFound)
			createNode(std::pair<int,int>(current->coordinate.first - 1, current->coordinate.second));
		if (current->coordinate.second > 0 && !objectFound)
			createNode(std::pair<int,int>(current->coordinate.first, current->coordinate.second - 1));

		if (!objectFound)
			setNextNode();
	}
}

void PathfindingObject::createNode(const std::pair<int,int> coordinate)
{
	if (!visited[coordinate.first][coordinate.second])
	{
		if (!grid->gridUnits[coordinate.second][coordinate.first].occupied)
		{
			NodeObject* node = new NodeObject();
			node->coordinate = coordinate;
			node->distanceToStart = current->distanceToStart + 1;
			node->parent = current;
			open.push(node);
		}
		visited[coordinate.first][coordinate.second] = true;

		checkObjectFound(coordinate);
	}
}

void PathfindingObject::checkObjectFound(std::pair<int,int> coordinate)
{
	switch (objectType)
	{
		case ObjectType::tree:
		{
			if (grid->HasTree(coordinate.first, coordinate.second))
				objectFound = true;
			break;
		}

		case ObjectType::idleWorker:
		{
			for (BoneAnimated* object : grid->gridUnits[coordinate.second][coordinate.first].movingObjects)
			{
				try
				{
					Worker* worker = dynamic_cast<Worker*>(object);
					if (worker)
						if (worker->state == State::idle)
							objectFound = true;
				}
				catch (const std::exception& e) {} // Not an exception. Expected behavior.
			}
			break;
		}

		case ObjectType::idleBuilding:
		{
			if (grid->HasBuilding(coordinate.first, coordinate.second))
			{
				Building* building = findBuildingReference(coordinate);
				if (building != nullptr)
					if (building->entranceX == coordinate.first && building->entranceY == coordinate.second) 
						if (building->WorkersRequired() > 0)
							objectFound = true;
			}
			break;
		}

		case ObjectType::unusedWood:
		{
			if (grid->HasBuilding(coordinate.first, coordinate.second))
			{
				Building* building = findBuildingReference(coordinate);
				if (building != nullptr)
					if (building->entranceX == coordinate.first && building->entranceY == coordinate.second) 
						if (building->UnusedWoodBuildingMaterial() > 0)
							objectFound = true;
			}
			break;
		}


		case ObjectType::unusedStone:
		{
			if (grid->HasBuilding(coordinate.first, coordinate.second))
			{
				Building* building = findBuildingReference(coordinate);
				if (building != nullptr)
					if (building->entranceX == coordinate.first && building->entranceY == coordinate.second) 
						if (building->UnusedStoneBuildingMaterial() > 0)
							objectFound = true;
			}
			break;
		}

		case ObjectType::woodRequired:
		{
			if (grid->HasBuilding(coordinate.first, coordinate.second))
			{
				Building* building = findBuildingReference(coordinate);
				if (building != nullptr)
					if (building->entranceX == coordinate.first && building->entranceY == coordinate.second) 
						if (building->WoodBuildingMaterialRequired() > 0)
							objectFound = true;
			}
			break;
		}

		case ObjectType::stoneRequired:
		{
			if (grid->HasBuilding(coordinate.first, coordinate.second))
			{
				Building* building = findBuildingReference(coordinate);
				if (building != nullptr)
					if (building->entranceX == coordinate.first && building->entranceY == coordinate.second) 
						if (building->StoneBuildingMaterialRequired() > 0)
							objectFound = true;
			}
			break;
		}

		case ObjectType::edge:
		{
			if (coordinate.first == maxX ||
				coordinate.first == 0 ||
				coordinate.second == maxY ||
				coordinate.second == 0)
			{
				objectFound = true;
			}
			break;
		}
	}
	
	if (objectFound)
	{
		if (coordinate == start->coordinate)
		{
			destination = start;
		}
		else
		{
			destination = new NodeObject();
			destination->coordinate = coordinate;
			destination->parent = current;
		}
		current = destination;
	}
}

void PathfindingObject::setNextNode()
{
	if (!open.empty())
	{
		current = open.top();
		open.pop();
		closed.push_front(current);
	}
	else
		unreachable = true;
}

Building* PathfindingObject::findBuildingReference(std::pair<int,int> coordinate) const
{
	Building* building;
	
	building = checkBuildingReference(coordinate);
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first + 1, coordinate.second));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first, coordinate.second + 1));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first - 1, coordinate.second));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first, coordinate.second - 1));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first + 1, coordinate.second + 1));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first + 1, coordinate.second - 1));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first - 1, coordinate.second + 1));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first - 1, coordinate.second - 1));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first + 2, coordinate.second));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first, coordinate.second + 2));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first - 2, coordinate.second));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first, coordinate.second - 2));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first + 2, coordinate.second + 2));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first + 2, coordinate.second - 2));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first - 2, coordinate.second + 2));
	if (building != nullptr)
		return building;

	building = checkBuildingReference(std::pair<int,int>(coordinate.first - 2, coordinate.second - 2));
	if (building != nullptr)
		return building;
	
	return nullptr;
}

Building* PathfindingObject::checkBuildingReference(std::pair<int,int> coordinate) const
{
	if (coordinate.first >= 0 &&
		coordinate.first <= maxX &&
		coordinate.second >= 0 &&
		coordinate.second <= maxY)
		for (GameObject* object : grid->gridUnits[coordinate.second][coordinate.first].objects)
			try
			{
				Building* building = dynamic_cast<Building*>(object);
				if (building)
					return building;
			}
			catch (const std::exception& e) {} // Not an exception. Expected behavior.
	return nullptr;
}
