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
			{
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
			}

			case ObjectType::idleWorker:
			{
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
			}

			case ObjectType::idleBuilding:
			{
				for (GameObject* object : grid->gridUnits[destination->coordinate.second][destination->coordinate.first].objects)
				{
					try
					{
						Building* building = dynamic_cast<Building*>(object);
						if (building)
							return object;
					}
					catch (const std::exception& e) {} // Not an exception, expected behavior...
				}
				break;
			}

			case ObjectType::unusedWood:
			{
				for (GameObject* object : grid->gridUnits[destination->coordinate.second][destination->coordinate.first].objects)
				{
					try
					{
						Building* building = dynamic_cast<Building*>(object);
						if (building)
							if (building->woodStored - building->woodRequired > 0)
								return object;
					}
					catch (const std::exception& e) {} // Not an exception, expected behavior...
				}
				break;
			}

			case ObjectType::unusedStone:
			{
				for (GameObject* object : grid->gridUnits[destination->coordinate.second][destination->coordinate.first].objects)
				{
					try
					{
						Building* building = dynamic_cast<Building*>(object);
						if (building)
							if (building->stoneStored - building->stoneRequired > 0)
								return object;
					}
					catch (const std::exception& e) {} // Not an exception, expected behavior...
				}
				break;
			}

			case ObjectType::woodRequired:
			{
				for (GameObject* object : grid->gridUnits[destination->coordinate.second][destination->coordinate.first].objects)
				{
					try
					{
						Building* building = dynamic_cast<Building*>(object);
						if (building)
							if (building->woodRequired > building->woodStored + building->woodOnTheWay)
								return object;
					}
					catch (const std::exception& e) {} // Not an exception, expected behavior...
				}
				break;
			}

			case ObjectType::stoneRequired:
			{
				for (GameObject* object : grid->gridUnits[destination->coordinate.second][destination->coordinate.first].objects)
				{
					try
					{
						Building* building = dynamic_cast<Building*>(object);
						if (building)
							if (building->stoneRequired > building->stoneStored + building->stoneOnTheWay)
								return object;
					}
					catch (const std::exception& e) {} // Not an exception, expected behavior...
				}
				break;
			}

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
				catch (const std::exception& e) {} // Not an exception, expected behavior...
			}
			break;
		}

		case ObjectType::idleBuilding:
		{
			for (GameObject* object : grid->gridUnits[coordinate.second][coordinate.first].objects)
			{
				try
				{
					Building* building = dynamic_cast<Building*>(object);
					if (building)
						if (building->workersPresent < building->workersRequired)
							objectFound = true;
				}
				catch (const std::exception& e) {} // Not an exception, expected behavior...
			}
			break;
		}

		case ObjectType::unusedWood:
			{
				for (GameObject* object : grid->gridUnits[coordinate.second][coordinate.first].objects)
				{
					try
					{
						Building* building = dynamic_cast<Building*>(object);
						if (building)
							if (building->woodStored - building->woodRequired > 0)
								objectFound = true;
					}
					catch (const std::exception& e) {} // Not an exception, expected behavior...
				}
				break;
			}


		case ObjectType::unusedStone:
			{
				for (GameObject* object : grid->gridUnits[coordinate.second][coordinate.first].objects)
				{
					try
					{
						Building* building = dynamic_cast<Building*>(object);
						if (building)
							if (building->stoneStored - building->stoneRequired > 0)
								objectFound = true;
					}
					catch (const std::exception& e) {} // Not an exception, expected behavior...
				}
				break;
			}

		case ObjectType::woodRequired:
			{
				for (GameObject* object : grid->gridUnits[coordinate.second][coordinate.first].objects)
				{
					try
					{
						Building* building = dynamic_cast<Building*>(object);
						if (building)
							if (building->woodRequired > building->woodStored + building->woodOnTheWay)
								objectFound = true;
					}
					catch (const std::exception& e) {} // Not an exception, expected behavior...
				}
				break;
			}

		case ObjectType::stoneRequired:
			{
				for (GameObject* object : grid->gridUnits[coordinate.second][coordinate.first].objects)
				{
					try
					{
						Building* building = dynamic_cast<Building*>(object);
						if (building)
							if (building->stoneRequired > building->stoneStored + building->stoneOnTheWay)
								objectFound = true;
					}
					catch (const std::exception& e) {} // Not an exception, expected behavior...
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
