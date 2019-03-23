#pragma once

#include <queue>
#include <forward_list>

#include "grid.h"
#include "globals.h"
#include "game_object.h"

typedef std::pair<unsigned short, unsigned short> Coordinate;

struct NodeObject
{
	Coordinate coordinate;
	unsigned short distanceToStart;
	NodeObject* parent;
};

struct NodeCompareObject 
{
	bool operator() (const NodeObject* node1, const NodeObject* node2)
	{
		if (node1->distanceToStart > node2->distanceToStart)
			return true;
		if (node1->distanceToStart < node2->distanceToStart)
			return false;

		return false;
	}
};

template <typename T>

class PathfindingObject
{
public:
	PathfindingObject() {};
	PathfindingObject(Grid* aGrid, const Coordinate XYstart, const T* objectType)
	{
		grid = aGrid;
		type = typeid(objectType).name();

		start = new NodeObject();
		start->coordinate = XYstart;
		start->distanceToStart = 0;

		closed.push_front(start);

	};

	~PathfindingObject()
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
	};

	void FindClosest()
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
	};

	std::list<Coordinate> GetPath()
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
	};

private:
	std::forward_list<NodeObject*> closed; // maybe not needed, only for storing pointers for deleting
	std::priority_queue<NodeObject*, std::deque<NodeObject*>, NodeCompareObject> open;
	bool visited[MAP_WIDTH][MAP_HEIGHT]{ false };

	Grid* grid;
	NodeObject* start;
	NodeObject* current;
	auto type;

	bool objectFound = false;
	NodeObject* destination;

	const unsigned short maxX = (unsigned short)MAP_WIDTH - 1;
	const unsigned short maxY = (unsigned short)MAP_HEIGHT - 1;

	void createNode(const Coordinate coordinate)
	{
		if (!visited[coordinate.first][coordinate.second])
		{
			if (!grid->gridUnits[coordinate.second][coordinate.first]->occupied)
			{
				NodeObject* node = new NodeObject();
				node->coordinate = coordinate;
				node->distanceToStart = current->distanceToStart++;
				node->parent = current;
				open.push(node);
			}
			visited[coordinate.first][coordinate.second] = true;

			checkObjectFound(coordinate);
		}
	};

	void checkObjectFound(Coordinate coordinate) 
	{
		for (GameObject* listItem : grid->gridUnits[coordinate.second][coordinate.first]->objects)
		{
			if (typeid(listItem) == type)
			{
				objectFound = true;
				destination = new Node();
				destination->coordinate = coordinate;
				destination->parent = current;
				break;
			}
		}
	};

	bool setNextNode()
	{
		if (!open.empty())
		{
			current = open.top();
			open.pop();
			closed.push_front(current);
			return true;
		}
		return false;
	};
};

