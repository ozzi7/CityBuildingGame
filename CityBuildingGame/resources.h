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
	void AddWorkerTask(Building* workplace);
	void RemoveIdleWorker(Worker* idleWorker);
	void RemoveWorkerTask(Building* idleBuilding);

	void AddLumberjack(Lumberjack* lumberjack);
	void AddWorker(Worker* worker);
	void AddDwelling(Dwelling* dwelling);
	void AddLumberjackHut(LumberjackHut* lumberjackHut);
	void RemoveObject(GameObject* gameObject);

	Worker* GetIdleWorker();
	Building* GetWorkerTask();
	std::vector<Worker*> GetIdleWorkers(int nofWorkersNeeded);

	std::list<Worker*> IdleWorkers = std::list<Worker*>();
	std::list<Building*> WorkerTasks = std::list<Building*>();

	std::list<Lumberjack*> Lumberjacks = std::list<Lumberjack*>();
	std::list<Worker*> Workers = std::list<Worker*>();
	std::list<Dwelling*> Dwellings = std::list<Dwelling*>();
	std::list<LumberjackHut*> LumberjackHuts = std::list<LumberjackHut*>();
};
