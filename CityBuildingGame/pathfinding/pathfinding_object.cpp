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
		for (GameObject* object : grid->gridUnits[destination->coordinate.second][destination->coordinate.first].objects)
		{
			if (objectType == ObjectType::tree)
			{
				try
				{
					Tree* tree = dynamic_cast<Tree*>(object);
					return object;
				}
				catch (const std::exception& e) {} // Not an exception, expected behavior...
			}
		}
	}
	return nullptr;
}

void PathfindingObject::calculatePath()
{
	while (!objectFound && !unreachable)
	{
		if (current->coordinate.first < maxX)
			createNode(std::pair<int,int>(current->coordinate.first + 1, current->coordinate.second));
		if (current->coordinate.second < maxY)
			createNode(std::pair<int,int>(current->coordinate.first, current->coordinate.second + 1));
		if (current->coordinate.first > 0)
			createNode(std::pair<int,int>(current->coordinate.first - 1, current->coordinate.second));
		if (current->coordinate.second > 0)
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
		if (grid->gridUnits[coordinate.second][coordinate.first].hasTree)
		{
			objectFound = true;
			destination = new NodeObject();
			destination->coordinate = coordinate;
			destination->parent = current;
		}
		break;

	case ObjectType::edge:
		if (coordinate.first == maxX ||
			coordinate.first == 0 ||
			coordinate.second == maxY ||
			coordinate.second == 0)
		{
			objectFound = true;
			destination = new NodeObject();
			destination->coordinate = coordinate;
			destination->parent = current;
		}
		break;
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
