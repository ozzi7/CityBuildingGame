#pragma once

#include <queue>
#include <forward_list>

#include <grid.h>
#include <globals.h>

typedef std::pair<unsigned short, unsigned short> Coordinate;

enum ObjectType { edge, tree };

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

class PathfindingObject
{
public:
	PathfindingObject() {};
	PathfindingObject(Grid* aGrid, const Coordinate XYstart);

	~PathfindingObject();

	void FindClosestTree();
	void FindClosestEdge();

	std::list<Coordinate> GetPath();
	GameObject* GetDestinationObject();
private:
	std::forward_list<NodeObject*> closed; // maybe not needed, only for storing pointers for deleting
	std::priority_queue<NodeObject*, std::deque<NodeObject*>, NodeCompareObject> open;
	bool visited[MAP_WIDTH][MAP_HEIGHT]{ false };

	Grid* grid;
	NodeObject* start;
	NodeObject* current;
	ObjectType objectType;

	bool objectFound = false;
	bool unreachable = false;
	NodeObject* destination = nullptr;

	const unsigned short maxX = (unsigned short)MAP_WIDTH - 1;
	const unsigned short maxY = (unsigned short)MAP_HEIGHT - 1;

	void calculatePath();
	void createNode(const Coordinate coordinate);
	void checkObjectFound(Coordinate coordinate);
	void setNextNode();
};

