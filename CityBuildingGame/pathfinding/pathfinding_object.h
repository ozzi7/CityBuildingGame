#pragma once

#include <queue>
#include <forward_list>

#include <grid.h>
#include <globals.h>

enum class ObjectType { edge, tree, idleWorker, idleBuilding, unusedWood, unusedStone, woodRequired, stoneRequired };

struct NodeObject
{
	std::pair<int,int> coordinate;
	int distanceToStart{};
	NodeObject* parent{};
};

struct NodeCompareObject
{
	bool operator()(const NodeObject* node1, const NodeObject* node2) const
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
	PathfindingObject(Grid* aGrid, std::pair<int,int> XYstart);

	~PathfindingObject();

	void FindClosestTree();
	void FindClosestEdge();
	void FindClosestIdleWorker();
	void FindClosestIdleBuilding();
	void FindClosestUnusedWood();
	void FindClosestUnusedStone();
	void FindClosestWoodRequired();
	void FindClosestStoneRequired();

	std::list<std::pair<int,int>> GetPath();
	GameObject* GetDestinationObject() const;
private:
	std::forward_list<NodeObject*> closed; // maybe not needed, only for storing pointers for deleting
	std::priority_queue<NodeObject*, std::deque<NodeObject*>, NodeCompareObject> open;
	bool visited[MAP_WIDTH][MAP_HEIGHT]{false};

	Grid* grid{};
	NodeObject* start{};
	NodeObject* current{};
	ObjectType objectType;

	bool objectFound = false;
	bool unreachable = false;
	NodeObject* destination = nullptr;

	const int maxX = (int)(MAP_WIDTH - 1);
	const int maxY = (int)(MAP_HEIGHT - 1);

	void calculatePath();
	void createNode(std::pair<int,int> coordinate);
	void checkObjectFound(std::pair<int,int> coordinate);
	void setNextNode();
	
	Building* findBuildingReference(std::pair<int,int> coordinate) const;
	Building* checkBuildingReference(std::pair<int,int> coordinate) const;
};
