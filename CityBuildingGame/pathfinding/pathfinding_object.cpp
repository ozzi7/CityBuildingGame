#include "stdafx.h"
#include "pathfinding_object.h"

PathfindingObject::PathfindingObject(Grid * aGrid, const Coordinate XYstart)
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
	{
		delete node;
	}
	while (!open.empty())
	{
		current = open.top();
		open.pop();
		delete current;
	}
	if (destination)
		delete destination;
}

void PathfindingObject::FindClosestTree()
{
	objectType = tree;
	calculatePath();
}

void PathfindingObject::FindClosestEdge()
{
	objectType = edge;
	calculatePath();
}

std::list<Coordinate> PathfindingObject::GetPath()
{
	std::list<Coordinate> path;
	if (objectFound)
	{
		current = destination;
		while (current->parent != nullptr) {
			path.push_front(current->coordinate);
			current = current->parent;
		};
	}
	return path;
}

GameObject* PathfindingObject::GetDestinationObject()
{	
	if (objectFound) {
		for (GameObject* object : grid->gridUnits[destination->coordinate.second][destination->coordinate.first]->objects)
		{
			if (objectType == tree)
			{
				try {
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
			createNode(Coordinate(current->coordinate.first + 1, current->coordinate.second));
		if (current->coordinate.second < maxY)
			createNode(Coordinate(current->coordinate.first, current->coordinate.second + 1));
		if (current->coordinate.first > 0)
			createNode(Coordinate(current->coordinate.first - 1, current->coordinate.second));
		if (current->coordinate.second > 0)
			createNode(Coordinate(current->coordinate.first, current->coordinate.second - 1));

		setNextNode();
	}
}

void PathfindingObject::createNode(const Coordinate coordinate)
{
	if (!visited[coordinate.first][coordinate.second])
	{
		if (!grid->gridUnits[coordinate.second][coordinate.first]->occupied)
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

void PathfindingObject::checkObjectFound(Coordinate coordinate)
{
	switch (objectType)
	{
	case tree:
		if (grid->gridUnits[coordinate.second][coordinate.first]->hasTree)
		{
			objectFound = true;
			destination = new NodeObject();
			destination->coordinate = coordinate;
			destination->parent = current;
		}
		break;

	case edge:
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
