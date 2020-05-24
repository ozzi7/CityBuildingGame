#include "stdafx.h"
#include "pathfinding.h"

Pathfinding::Pathfinding(Grid* aGrid, const std::pair<int,int> XYstart, const std::pair<int,int> XYdestination)
{
	grid = aGrid;

	destination = new Node();
	destination->coordinate = XYdestination;
	start = new Node();
	start->coordinate = XYstart;
	start->distanceToStart = 0;
	start->distanceToDestination = distanceToDestination(XYstart);
	start->distanceTotal = start->distanceToStart + start->distanceToDestination;
	start->destination = destination->coordinate;
	visited[XYstart.first][XYstart.second] = true;
}

void Pathfinding::CalculatePath()
{
	auto startTime = std::chrono::high_resolution_clock::now();

	open.push(start);
		if (start->coordinate == destination->coordinate)
			pathFound = true;
	setNextNode();
	
	while (!pathFound && !unreachable)
	{
		if (current->coordinate.first < maxX)
			createNode(std::pair<int,int>(current->coordinate.first + 1, current->coordinate.second));
		if (current->coordinate.second < maxY)
			createNode(std::pair<int,int>(current->coordinate.first, current->coordinate.second + 1));
		if (current->coordinate.first > 0)
			createNode(std::pair<int,int>(current->coordinate.first - 1, current->coordinate.second));
		if (current->coordinate.second > 0)
			createNode(std::pair<int,int>(current->coordinate.first, current->coordinate.second - 1));

		if(!pathFound)
			setNextNode();
	}

	auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	long elapsedTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count();
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "Executed Pathfinding from " + 
																			 std::to_string(start->coordinate.first) + "|" + 
																			 std::to_string(start->coordinate.second) + " to " +
																			 std::to_string(destination->coordinate.first) + "|" +
																			 std::to_string(destination->coordinate.second) + " in " +
																			 std::to_string(elapsedTimeMicroseconds) + " microseconds; PathFound: " +
																			 std::to_string(pathFound)));
}

std::list<std::pair<int,int>> Pathfinding::GetPath() const
{
	std::list<std::pair<int, int>> path;
	Node* currentPath = current;
	if (pathFound)
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

void Pathfinding::createNode(const std::pair<int,int> coordinate)
{
	if (!visited[coordinate.first][coordinate.second])
	{
		if (!grid->gridUnits[coordinate.second][coordinate.first].occupied ||
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

int Pathfinding::distanceToDestination(const std::pair<int,int> coordinate) const
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
