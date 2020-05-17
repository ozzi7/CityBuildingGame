#include "stdafx.h"
#include "pathfinding_object.h"

PathfindingObject::PathfindingObject(Grid* aGrid, const std::pair<int,int> XYstart)
{
	grid = aGrid;

	start = new NodeObject();
	start->coordinate = XYstart;
	start->distanceToStart = 0;

	closed.push_front(start);

	current = start;
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

	delete destination;
}

void PathfindingObject::FindClosestTree()
{
	objectType = ObjectType::tree;
	calculatePath();
}

void PathfindingObject::FindClosestEdge()
{
	objectType = ObjectType::edge;
	calculatePath();
}

void PathfindingObject::FindClosestIdleWorker()
{
	objectType = ObjectType::idleWorker;
	calculatePath();
}

void PathfindingObject::FindClosestIdleBuilding()
{
	objectType = ObjectType::idleBuilding;
	calculatePath();
}

void PathfindingObject::FindClosestUnusedWood()
{
	objectType = ObjectType::unusedWood;
	calculatePath();
}

void PathfindingObject::FindClosestUnusedStone()
{
	objectType = ObjectType::unusedStone;
	calculatePath();
}

void PathfindingObject::FindClosestWoodRequired()
{
	objectType = ObjectType::woodRequired;
	calculatePath();
}

void PathfindingObject::FindClosestStoneRequired()
{
	objectType = ObjectType::stoneRequired;
	calculatePath();
}

std::list<std::pair<int,int>> PathfindingObject::GetPath()
{
	std::list<std::pair<int,int>> path;
	if (objectFound)
	{
		current = destination;
		while (current->parent != nullptr)
		{
			path.push_front(current->coordinate);
			current = current->parent;
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
				for (GameObject* object : grid->gridUnits[destination->coordinate.second][destination->coordinate.first].movingObjects)
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
	while (!objectFound && !unreachable)
	{
		if (current->coordinate.first < maxX && !objectFound)
			createNode(std::pair<int,int>(current->coordinate.first + 1, current->coordinate.second));
		if (current->coordinate.second < maxY && !objectFound)
			createNode(std::pair<int,int>(current->coordinate.first, current->coordinate.second + 1));
		if (current->coordinate.first > 0 && !objectFound)
			createNode(std::pair<int,int>(current->coordinate.first - 1, current->coordinate.second));
		if (current->coordinate.second > 0 && !objectFound)
			createNode(std::pair<int,int>(current->coordinate.first, current->coordinate.second - 1));

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
			if (grid->gridUnits[coordinate.second][coordinate.first].hasTree)
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
			if (!grid->gridUnits[coordinate.second][coordinate.first].hasTree) // TODO: Use hasBuilding flag instead 
			{
				Building* building = findBuildingReference(coordinate);
				if (building != nullptr)
					if (building->entranceX == coordinate.first && building->entranceY == coordinate.second) 
						if (building->workersPresent < building->workersRequired)
							objectFound = true;
			}
			break;
		}

		case ObjectType::unusedWood:
		{
			if (!grid->gridUnits[coordinate.second][coordinate.first].hasTree) // TODO: Use hasBuilding flag instead 
			{
				Building* building = findBuildingReference(coordinate);
				if (building != nullptr)
					if (building->entranceX == coordinate.first && building->entranceY == coordinate.second) 
						if (building->woodStored - building->woodRequired > 0)
							objectFound = true;
			}
			break;
		}


		case ObjectType::unusedStone:
		{
			if (!grid->gridUnits[coordinate.second][coordinate.first].hasTree) // TODO: Use hasBuilding flag instead 
			{
				Building* building = findBuildingReference(coordinate);
				if (building != nullptr)
					if (building->entranceX == coordinate.first && building->entranceY == coordinate.second) 
						if (building->stoneStored - building->stoneRequired > 0)
							objectFound = true;
			}
			break;
		}

		case ObjectType::woodRequired:
		{
			if (!grid->gridUnits[coordinate.second][coordinate.first].hasTree) // TODO: Use hasBuilding flag instead 
			{
				Building* building = findBuildingReference(coordinate);
				if (building != nullptr)
					if (building->entranceX == coordinate.first && building->entranceY == coordinate.second) 
						if (building->woodRequired > building->woodStored + building->woodOnTheWay)
							objectFound = true;
			}
			break;
		}

		case ObjectType::stoneRequired:
		{
			if (!grid->gridUnits[coordinate.second][coordinate.first].hasTree) // TODO: Use hasBuilding flag instead 
			{
				Building* building = findBuildingReference(coordinate);
				if (building != nullptr)
					if (building->entranceX == coordinate.first && building->entranceY == coordinate.second) 
						if (building->stoneRequired > building->stoneStored + building->stoneOnTheWay)
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
		destination = new NodeObject();
		destination->coordinate = coordinate;
		destination->parent = current;
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
