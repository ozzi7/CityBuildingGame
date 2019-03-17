#pragma once

#include <queue>
#include <deque>
#include <forward_list>
#include <algorithm>

#include "grid.h"
#include "globals.h"

#define max(x,y) ((x) > (y) ? (x) : (y))



typedef std::pair<unsigned short, unsigned short> Coordinate;

struct Node
{
	Coordinate coordinate;
	unsigned short distanceToStart;
	unsigned short distanceToDestination;
	unsigned short distanceTotal;
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

		double distance1 =
			std::sqrt(
				std::pow((node1->coordinate.first - node1->destination.first), 2) +
				std::pow((node1->coordinate.second - node1->destination.second), 2)
			);
		double distance2 =
			std::sqrt(
				std::pow((node2->coordinate.first - node1->destination.first), 2) +
				std::pow((node2->coordinate.second - node1->destination.second), 2)
			);

		if (distance1 > distance2)
			return true;
		//if (distance1 < distance2)
		return false;

		//if (node1->coordinate.first > node2->coordinate.first)
		//	return true;
		//if (node1->coordinate.first < node2->coordinate.first)
		//	return false;
		//if (node1->coordinate.second > node2->coordinate.second)
		//	return true;
		//if (node1->coordinate.second < node2->coordinate.second)
		//	return false;

		//int xDistance1 = std::abs(node1->coordinate.first - node1->destination.first);
		//int yDistance1 = std::abs(node1->coordinate.second - node1->destination.second);
		//int xDistance2 = std::abs(node2->coordinate.first - node1->destination.first);
		//int yDistance2 = std::abs(node2->coordinate.second - node1->destination.second);
		
		//int max = max(max(xDistance1, yDistance1), max(xDistance2, yDistance2));
		//std::cout << max << "MAX \n";
		//if (xDistance1 == max) {
		//	std::cout << xDistance1 << " x1 \n";
		//	return true;
		//}
		//if (yDistance1 == max) {
		//	std::cout << yDistance1 << " y1 \n";
		//	return true;
		//}
		//if (xDistance2 == max) {
		//	std::cout << xDistance2 << " x2 \n";
		//	return false;
		//}
		//if (yDistance2 == max) {
		//	std::cout << yDistance2 << " y2 \n";
		//	return false;
		//}

		//// they are the same, so just pick one..
		//if (node1->coordinate.first > node2->coordinate.first)
		//	return true;
		
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
	bool visited[MAP_WIDTH][MAP_HEIGHT] { false };

	Grid* grid;
	Node* start;
	Node* destination;
	Node* current;

	const unsigned short maxX = (unsigned short)MAP_WIDTH - 1;
	const unsigned short maxY = (unsigned short)MAP_HEIGHT - 1;
	
	void createNode(const Coordinate coordinate);
	unsigned short distanceToDestination(const Coordinate coordinate);
	bool setNextNode();
	void adjustParentNode(Node* node);
};

