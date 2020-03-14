#pragma once
#include <list>
#include <vector>

class Worker;
class Building;
class Lumberjack;
class Dwelling;
class LumberjackHut;
class GameObject;

class Resources
{
public:
	Resources();
	~Resources();
	void AddIdleWorker(Worker* idleWorker);
	void AddIdleBuilding(Building* idleBuilding);

	void AddLumberjack(Lumberjack* lumberjack);
	void AddWorker(Worker* worker);
	void AddDwelling(Dwelling* dwelling);
	void AddLumberjackHut(LumberjackHut* lumberjackHut);
	void RemoveObject(GameObject* gameObject); 

	Worker* GetIdleWorker();
	Building* GetIdleBuilding();
	std::vector<Worker*> GetIdleWorkers(int nofWorkersNeeded);

	std::list<Worker*> idleWorkers = std::list<Worker*>();
	std::list<Building*> idleBuildings = std::list<Building*>();

	std::list<Lumberjack*> lumberjacks = std::list<Lumberjack*>();
	std::list<Worker*> workers = std::list<Worker*>();
	std::list<Dwelling*> dwellings = std::list<Dwelling*>();
	std::list<LumberjackHut*> lumberjackHuts = std::list<LumberjackHut*>();
};
