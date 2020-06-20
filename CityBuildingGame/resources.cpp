#include "stdafx.h"
#pragma once

#include "resources.h"

Resources::Resources() = default;
Resources::~Resources() = default;

void Resources::AddLumberjackHut(LumberjackHut* lumberjackHut)
{
	LumberjackHuts.push_back(lumberjackHut);
};
void Resources::AddDwelling(Dwelling* dwelling)
{
	Dwellings.push_back(dwelling);
};
void Resources::AddLumberjack(Lumberjack* lumberjack)
{
	Lumberjacks.push_back(lumberjack);
};
void Resources::AddWorker(Worker* worker)
{
	Workers.push_back(worker);
};
// not implicitly called on add worker
void Resources::AddIdleWorker(Worker* idleWorker)
{
	IdleWorkers.push_back(idleWorker);
};
// not implicitly called on addbuilding
void Resources::AddWorkerTask(Building* workplace)
{
	WorkerTasks.push_back(workplace);
}

// not implicitly called on RemoveObject
void Resources::RemoveIdleWorker(Worker* idleWorker)
{
	long pointer = (long)(void*)idleWorker;
	for (std::list<Worker*>::iterator it = IdleWorkers.begin();it != IdleWorkers.end(); ++it)
	{
		if (pointer == (long)(void*)(*it))
		{
			it = IdleWorkers.erase(it);
			return;
		}
	}
}

// not implicitly called on RemoveObject
void Resources::RemoveWorkerTask(Building* workplace)
{
	long pointer = (long)(void*)workplace;
	for (std::list<Building*>::iterator it = WorkerTasks.begin();it != WorkerTasks.end(); ++it)
	{
		if (pointer == (long)(void*)(*it))
		{
			it = WorkerTasks.erase(it);
			return;
		}
	}
};

void Resources::RemoveObject(GameObject* gameObject)
{
	long pointer = (long)(void*)gameObject;
	for (std::list<Lumberjack*>::iterator it = Lumberjacks.begin();it != Lumberjacks.end(); ++it)
	{
		if (pointer == (long)(void*)(*it))
		{
			it = Lumberjacks.erase(it);
			return;
		}
	}
	for (std::list<Worker*>::iterator it = Workers.begin(); it != Workers.end(); ++it)
	{
		if (pointer == (long)(void*)(*it))
		{
			it = Workers.erase(it);
			return;
		}
	}
	for (std::list<LumberjackHut*>::iterator it = LumberjackHuts.begin(); it != LumberjackHuts.end(); ++it)
	{
		if (pointer == (long)(void*)(*it))
		{
			it = LumberjackHuts.erase(it);
			return;
		}
	}
	for (std::list<Dwelling*>::iterator it = Dwellings.begin(); it != Dwellings.end(); ++it)
	{
		if (pointer == (long)(void*)(*it))
		{
			it = Dwellings.erase(it);
			return;
		}
	}
};

Worker* Resources::GetIdleWorker()
{
	Worker* last = nullptr;
	if (!IdleWorkers.empty())
	{
		last = IdleWorkers.back();
		IdleWorkers.pop_back();
	}
	return last;
};

Building* Resources::GetWorkerTask()
{
	Building* last = nullptr;
	if (!WorkerTasks.empty())
	{
		last = WorkerTasks.back();
		WorkerTasks.pop_back();
	}
	return last;
};

// returns empty vector if not enough idle workers found
std::vector<Worker*> Resources::GetIdleWorkers(int nofWorkersNeeded)
{
	std::vector<Worker*> workers = std::vector<Worker*>();

	if (IdleWorkers.size() >= nofWorkersNeeded)
	{
		for (int i = 0; i < nofWorkersNeeded; ++i)
			workers.push_back(GetIdleWorker());
	}
	return workers;
};
