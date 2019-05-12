#include "stdafx.h"
#include "pathfinding.h"

Pathfinding::Pathfinding(Grid* aGrid, const Coordinate XYstart, const Coordinate XYdestination)
{
	grid = aGrid;

	start = new Node();
	destination = new Node();
	start->coordinate = XYstart;
	destination->coordinate = XYdestination;
	start->distanceToStart = 0;
	start->distanceToDestination = distanceToDestination(XYstart);
	start->distanceTotal = start->distanceToStart + start->distanceToDestination;
	start->destination = destination->coordinate;
	visited[XYstart.first][XYstart.second] = true;
	closed.push_front(start);

	current = start;
}

void Pathfinding::CalculatePath()
{
	while (!pathFound && !unreachable)
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

std::list<Coordinate> Pathfinding::GetPath()
{
	std::list<std::pair<unsigned short, unsigned short>> path;
	if (pathFound)
	{
		while (current->parent != nullptr)
		{
			path.push_front(current->coordinate);
			current = current->parent;
		}
	}
	return path;
}

void Pathfinding::createNode(const Coordinate coordinate)
{
	if (!visited[coordinate.first][coordinate.second])
	{
		if (!grid->gridUnits[coordinate.second][coordinate.first]->occupied ||
			coordinate == destination->coordinate)
		{
			Node* node = new Node();
			node->coordinate = coordinate;
			node->distanceToStart = current->distanceToStart + 1;
			node->distanceToDestination = distanceToDestination(coordinate);
			node->distanceTotal = node->distanceToStart + node->distanceToDestination;
			node->parent = current;
			node->destination = destination->coordinate;
			//adjustParentNode(node); TODO
			//std::cout << "Inserting " << coordinate.first << '|' << coordinate.second << '\n';
			open.push(node);

			if (coordinate == destination->coordinate)
				pathFound = true;
		}
		visited[coordinate.first][coordinate.second] = true;
	}
}

unsigned short Pathfinding::distanceToDestination(const Coordinate coordinate) const
{
	return
		std::abs(coordinate.first - destination->coordinate.first) +
		std::abs(coordinate.second - destination->coordinate.second);
}

void Pathfinding::setNextNode()
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

void Pathfinding::adjustParentNode(Node* node)
{
	if (node->parent->parent != nullptr && node->parent->parent->parent != nullptr)
	{
		if (!(node->parent->parent->parent->coordinate.first + 1 > node->coordinate.first ||
			node->parent->parent->parent->coordinate.first - 1 < node->coordinate.first ||
			node->parent->parent->parent->coordinate.second + 1 > node->coordinate.second ||
			node->parent->parent->parent->coordinate.second - 1 < node->coordinate.second))
		{
			node->parent = node->parent->parent->parent;
			node->distanceToStart = node->parent->distanceToStart + 1;
			node->distanceTotal = node->distanceToStart + node->distanceToDestination;
		}
	}
}

Pathfinding::~Pathfinding()
{
	for (Node* node : closed)
		delete node;

	while (!open.empty())
	{
		current = open.top();
		open.pop();
		delete current;
	}

	delete destination;
}
