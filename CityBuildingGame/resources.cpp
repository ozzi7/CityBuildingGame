#include "stdafx.h"
#pragma once

#include "resources.h"

Resources::Resources() = default;
Resources::~Resources() = default;

void Resources::AddLumberjackHut(LumberjackHut* lumberjackHut)
{
	lumberjackHuts.push_back(lumberjackHut);
};
void Resources::AddDwelling(Dwelling* dwelling)
{
	dwellings.push_back(dwelling);
};
void Resources::AddLumberjack(Lumberjack* lumberjack)
{
	lumberjacks.push_back(lumberjack);
};
void Resources::AddWorker(Worker* worker)
{
	workers.push_back(worker);
};
// not implicitly called on add worker
void Resources::AddIdleWorker(Worker* idleWorker)
{
	idleWorkers.push_back(idleWorker);
};
// not implicitly called on addbuilding
void Resources::AddWorkerTask(Building* workplace)
{
	workerTasks.push_back(workplace);
}

// not implicitly called on RemoveObject
void Resources::RemoveIdleWorker(Worker* idleWorker)
{
	long pointer = (long)(void*)idleWorker;
	for (std::list<Worker*>::iterator it = idleWorkers.begin();it != idleWorkers.end(); ++it)
	{
		if (pointer == (long)(void*)(*it))
		{
			it = idleWorkers.erase(it);
			return;
		}
	}
}

// not implicitly called on RemoveObject
void Resources::RemoveWorkerTask(Building* workplace)
{
	long pointer = (long)(void*)workplace;
	for (std::list<Building*>::iterator it = workerTasks.begin();it != workerTasks.end(); ++it)
	{
		if (pointer == (long)(void*)(*it))
		{
			it = workerTasks.erase(it);
			return;
		}
	}
};

void Resources::RemoveObject(GameObject* gameObject)
{
	long pointer = (long)(void*)gameObject;
	for (std::list<Lumberjack*>::iterator it = lumberjacks.begin();it != lumberjacks.end(); ++it)
	{
		if (pointer == (long)(void*)(*it))
		{
			it = lumberjacks.erase(it);
			return;
		}
	}
	for (std::list<Worker*>::iterator it = workers.begin(); it != workers.end(); ++it)
	{
		if (pointer == (long)(void*)(*it))
		{
			it = workers.erase(it);
			return;
		}
	}
	for (std::list<LumberjackHut*>::iterator it = lumberjackHuts.begin(); it != lumberjackHuts.end(); ++it)
	{
		if (pointer == (long)(void*)(*it))
		{
			it = lumberjackHuts.erase(it);
			return;
		}
	}
	for (std::list<Dwelling*>::iterator it = dwellings.begin(); it != dwellings.end(); ++it)
	{
		if (pointer == (long)(void*)(*it))
		{
			it = dwellings.erase(it);
			return;
		}
	}
};

Worker* Resources::GetIdleWorker()
{
	Worker* last = nullptr;
	if (!idleWorkers.empty())
	{
		last = idleWorkers.back();
		idleWorkers.pop_back();
	}
	return last;
};

Building* Resources::GetWorkerTask()
{
	Building* last = nullptr;
	if (!workerTasks.empty())
	{
		last = workerTasks.back();
		workerTasks.pop_back();
	}
	return last;
};

// returns empty vector if not enough idle workers found
std::vector<Worker*> Resources::GetIdleWorkers(int nofWorkersNeeded)
{
	std::vector<Worker*> workers = std::vector<Worker*>();

	if (idleWorkers.size() >= nofWorkersNeeded)
	{
		for (int i = 0; i < nofWorkersNeeded; ++i)
			workers.push_back(GetIdleWorker());
	}
	return workers;
};
