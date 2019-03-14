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
	while (!(current->coordinate.first == destination->coordinate.first &&
		current->coordinate.second == destination->coordinate.second))
	{
		if (current->coordinate.first < maxX)
			if (!visited[current->coordinate.first + 1][current->coordinate.second])
			{
				if (!grid->gridUnits[current->coordinate.second][current->coordinate.first + 1]->occupied)
					createNode(Coordinate(current->coordinate.first + 1, current->coordinate.second));
				visited[current->coordinate.first + 1][current->coordinate.second] = true;
			}

		if (current->coordinate.second < maxY)
			if (!visited[current->coordinate.first][current->coordinate.second + 1])
			{
				if (!grid->gridUnits[current->coordinate.second + 1][current->coordinate.first]->occupied)
					createNode(Coordinate(current->coordinate.first, current->coordinate.second + 1));
				visited[current->coordinate.first][current->coordinate.second + 1] = true;
			}

		if (current->coordinate.first > 0)
			if (!visited[current->coordinate.first - 1][current->coordinate.second])
			{
				if (!grid->gridUnits[current->coordinate.second][current->coordinate.first - 1]->occupied)
					createNode(Coordinate(current->coordinate.first - 1, current->coordinate.second));
				visited[current->coordinate.first - 1][current->coordinate.second] = true;
			}

		if (current->coordinate.second > 0)
			if (!visited[current->coordinate.first][current->coordinate.second - 1])
			{
				if (!grid->gridUnits[current->coordinate.second - 1][current->coordinate.first]->occupied)
					createNode(Coordinate(current->coordinate.first, current->coordinate.second - 1));
				visited[current->coordinate.first][current->coordinate.second - 1] = true;
			}
		if (!setNextNode())
			break;
	}
}

std::list<Coordinate> Pathfinding::GetPath()
{
	std::list<std::pair<unsigned short, unsigned short>> path;
	while (current->parent != nullptr) {
		path.push_front(current->coordinate);
		current = current->parent;
	};
	
	return path;
}

void Pathfinding::createNode(const Coordinate coordinate)
{
	Node* node = new Node();
	node->coordinate.first = coordinate.first;
	node->coordinate.second = coordinate.second;
	node->distanceToStart = current->distanceToStart + 1;
	node->distanceToDestination = distanceToDestination(node->coordinate);
	node->distanceTotal = node->distanceToStart + node->distanceToDestination;
	node->parent = current;
	node->destination = destination->coordinate;
	adjustParentNode(node);
	open.push(node);
}

unsigned short Pathfinding::distanceToDestination(const Coordinate coordinate)
{
	return
	std::abs(coordinate.first - destination->coordinate.first) +
	std::abs(coordinate.second - destination->coordinate.second);
}

bool Pathfinding::setNextNode()
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