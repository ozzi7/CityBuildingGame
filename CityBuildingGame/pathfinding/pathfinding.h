#pragma once

#include <queue>
#include <deque>
#include <forward_list>
#include <algorithm>

#include <grid.h>
#include <globals.h>

#define max(x,y) ((x) > (y) ? (x) : (y))

typedef std::pair<unsigned short, unsigned short> Coordinate;

struct Node
{
	Coordinate coordinate;
	unsigned short distanceToStart = 0;
	unsigned short distanceToDestination = 0;
	unsigned short distanceTotal = 0;
	Node* parent;
	Coordinate destination;
};

struct NodeCompare {
	bool operator() (const Node* node1, const Node* node2)
	{
		if (node1->distanceTotal > node2->distanceTotal)
			return true;
		if (node1->distanceTotal < node2->distanceTotal)
			return false;

		//double distance1 =
		//	std::sqrt(
		//		std::pow((node1->coordinate.first - node1->destination.first), 2) +
		//		std::pow((node1->coordinate.second - node1->destination.second), 2)
		//	);
		//double distance2 =
		//	std::sqrt(
		//		std::pow((node2->coordinate.first - node1->destination.first), 2) +
		//		std::pow((node2->coordinate.second - node1->destination.second), 2)
		//	);

		//if (distance1 > distance2)
		//	return true;

		//return false;

		short xDistance1 = std::abs(node1->coordinate.first - node1->destination.first);
		short yDistance1 = std::abs(node1->coordinate.second - node1->destination.second);
		short xDistance2 = std::abs(node2->coordinate.first - node1->destination.first);
		short yDistance2 = std::abs(node2->coordinate.second - node1->destination.second);

		if (xDistance1 > yDistance1 &&
			xDistance1 > xDistance2 &&
			xDistance1 > yDistance2)
			return true;

		if (yDistance1 > xDistance1 &&
			yDistance1 > xDistance2 &&
			yDistance1 > yDistance2)
			return true;

		return false;
	}
};

class Pathfinding
{
public:
	Pathfinding(Grid* aGrid, const Coordinate XYstart, const Coordinate XYdestination);
	~Pathfinding();

	void CalculatePath();
	std::list<Coordinate> GetPath();

private:
	std::forward_list<Node*> closed; // maybe not needed, only for storing pointers for deleting
	std::priority_queue<Node*, std::deque<Node*>, NodeCompare> open;
	bool visited[MAP_WIDTH][MAP_HEIGHT]{ false };

	Grid* grid;
	Node* start;
	Node* destination = nullptr;
	Node* current;

	bool pathFound = false;
	bool unreachable = false;

	const unsigned short maxX = (unsigned short)MAP_WIDTH - 1;
	const unsigned short maxY = (unsigned short)MAP_HEIGHT - 1;

	void createNode(const Coordinate coordinate);
	unsigned short distanceToDestination(const Coordinate coordinate);
	void setNextNode();
	void adjustParentNode(Node* node);
};
