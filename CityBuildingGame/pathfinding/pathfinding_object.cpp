#include "stdafx.h"
#include "pathfinding_object.h"

inline PathfindingObject::PathfindingObject(Grid * aGrid, const Coordinate XYstart)
{
	grid = aGrid;

	start = new NodeObject();
	start->coordinate = XYstart;
	start->distanceToStart = 0;

	closed.push_front(start);

	current = start;
}

inline PathfindingObject::~PathfindingObject()
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
	delete destination;
}

inline void PathfindingObject::FindClosestTree()
{
	objectType = tree;
	calculatePath();
}

inline std::list<Coordinate> PathfindingObject::GetPath()
{
	std::list<std::pair<unsigned short, unsigned short>> path;
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

inline void PathfindingObject::calculatePath()
{
	while (!objectFound)
	{
		if (current->coordinate.first < maxX)
			createNode(Coordinate(current->coordinate.first + 1, current->coordinate.second));
		if (current->coordinate.second < maxY)
			createNode(Coordinate(current->coordinate.first, current->coordinate.second + 1));
		if (current->coordinate.first > 0)
			createNode(Coordinate(current->coordinate.first - 1, current->coordinate.second));
		if (current->coordinate.second > 0)
			createNode(Coordinate(current->coordinate.first, current->coordinate.second - 1));

		if (!setNextNode())
			break;
	}
}

inline void PathfindingObject::createNode(const Coordinate coordinate)
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
	}
}

bool PathfindingObject::setNextNode()
{
	if (!open.empty())
	{
		current = open.top();
		open.pop();
		closed.push_front(current);
		return true;
	}
	return false;
}
