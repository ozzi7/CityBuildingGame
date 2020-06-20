#include "stdafx.h"
#include "pathfinding.h"

Pathfinding::Pathfinding(Grid* aGrid, const std::pair<int,int> XYstart, const std::pair<int,int> XYdestination)
{
	grid = aGrid;

	destination = new Node();
	destination->coordinate = XYdestination;
	start = new Node();
	start->coordinate = XYstart;
	start->distanceToStart = 0.0f;
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
	/*for (auto coordinate : GetPath())
		loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "Final path: " + std::to_string(coordinate.first) + "|" + std::to_string(coordinate.second)));*/
}

std::list<std::pair<int,int>> Pathfinding::GetPath() const
{
	std::list<std::pair<int, int>> path;
	if (pathFound)
	{
		Node* currentPath = current;
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
			node->distanceToStart = current->distanceToStart + (grid->GetWalkingCost(current->coordinate.first, current->coordinate.second) * 0.5f + 
																grid->GetWalkingCost(coordinate.first, coordinate.second) * 0.5f);
			node->distanceToDestination = distanceToDestination(coordinate);
			node->distanceTotal = node->distanceToStart + node->distanceToDestination;
			node->parent = current;
			node->destination = destination->coordinate;
			//loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, std::to_string(coordinate.first) + "|" + std::to_string(coordinate.second)));
			open.push(node);
			visitedNodes.insert({generateID(coordinate), node});

			if (coordinate == destination->coordinate)
			{
				current = node;
				pathFound = true;
			}
		}
		visited[coordinate.first][coordinate.second] = true;
	}
	else
		adjustParentNode(coordinate);
}

float Pathfinding::distanceToDestination(const std::pair<int,int> coordinate) const
{
	return (float)(std::abs(coordinate.first - destination->coordinate.first) +
				   std::abs(coordinate.second - destination->coordinate.second)) * (1.0f / 1.5f); // 1.5f -> has to be fastest possible walking speed
}

void Pathfinding::setNextNode()
{
	if (!pathFound)
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
}

void Pathfinding::adjustParentNode(std::pair<int,int> coordinate)
{
	if (visitedNodes.find(generateID(coordinate)) != visitedNodes.end())
	{
		Node* node = visitedNodes.at(generateID(coordinate));

		if (node->parent != nullptr)
			// No need to check the node already has current as parent
			if (node->parent != current)
				// Change parent of current node to neighbor node, if neighbor is closer to start than current parent
				if (node->parent->distanceToStart > current->distanceToStart)
					node->parent = current;

	}
}

std::string Pathfinding::generateID(std::pair<int, int> coordinate) const
{
	return std::to_string(coordinate.first) + "/" + std::to_string(coordinate.second);
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
