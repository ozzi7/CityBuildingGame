#pragma once

#include <queue>
#include <deque>
#include <forward_list>
#include <algorithm>

#include <grid.h>
#include <globals.h>

#define max(x,y) ((x) > (y) ? (x) : (y))

struct Node
{
	std::pair<int,int> coordinate;
	float distanceToStart = 0.0f;
	float distanceToDestination = 0.0f;
	float distanceTotal = 0.0f;
	bool poppedFromOpen = false;
	Node* parent{};
	std::pair<int,int> destination;
};

struct NodeCompare
{
	bool operator()(const Node* node1, const Node* node2) const
	{
		if (node1->distanceTotal > node2->distanceTotal)
			return true;
		if (node1->distanceTotal < node2->distanceTotal)
			return false;


		int xDistance1 = std::abs(node1->coordinate.first - node1->destination.first);
		int yDistance1 = std::abs(node1->coordinate.second - node1->destination.second);
		int xDistance2 = std::abs(node2->coordinate.first - node1->destination.first);
		int yDistance2 = std::abs(node2->coordinate.second - node1->destination.second);

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
	Pathfinding(Grid* aGrid, std::pair<int,int> XYstart, std::pair<int,int> XYdestination);
	~Pathfinding();

	void CalculatePath();
	std::list<std::pair<int,int>> GetPath() const;

private:
	std::unordered_map<unsigned int, Node*> visitedNodes; // use generateID() as key
	std::priority_queue<Node*, std::deque<Node*>, NodeCompare> open;
	bool visited[MAP_WIDTH][MAP_HEIGHT]{false};

	Grid* grid;
	Node* start;
	Node* destination = nullptr;
	Node* current;

	bool pathFound = false;
	bool unreachable = false;

	const int maxX = (int)(MAP_WIDTH - 1);
	const int maxY = (int)(MAP_HEIGHT - 1);

	void createNode(std::pair<int,int> coordinate);
	float distanceToDestination(std::pair<int,int> coordinate) const;
	void setNextNode();
	void adjustParentNode(std::pair<int,int> coordinate);
	unsigned int generateID(std::pair<int,int> coordinate) const;
};
