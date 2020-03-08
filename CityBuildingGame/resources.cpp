#include "stdafx.h"
#pragma once

#include "resources.h"

Resources::Resources() {}
Resources::~Resources() {}

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
void Resources::AddSettler(Settler* settler)
{
	settlers.push_back(settler);
};
// not implicitly called on addsettler
void Resources::AddIdleSettler(Settler* idleSettler)
{
	idleSettlers.push_back(idleSettler);
};
// not implicitly called on addbuilding
void Resources::AddIdleBuilding(Building* idleBuilding)
{
	idleBuildings.push_back(idleBuilding);
};

void Resources::RemoveObject(GameObject* gameObject)
{
	long bla = (long)(void*)gameObject;
	for (std::list<Lumberjack*>::iterator it = lumberjacks.begin();it != lumberjacks.end(); ++it)
	{
		if (bla == (long)(void*)(*it))
		{
			it = lumberjacks.erase(it);
			return;
		}
	}
	for (std::list<Settler*>::iterator it = settlers.begin(); it != settlers.end(); ++it)
	{
		if (bla == (long)(void*)(*it))
		{
			it = settlers.erase(it);
			return;
		}
	}
	for (std::list<LumberjackHut*>::iterator it = lumberjackHuts.begin(); it != lumberjackHuts.end(); ++it)
	{
		if (bla == (long)(void*)(*it))
		{
			it = lumberjackHuts.erase(it);
			return;
		}
	}
	for (std::list<Dwelling*>::iterator it = dwellings.begin(); it != dwellings.end(); ++it)
	{
		if (bla == (long)(void*)(*it))
		{
			it = dwellings.erase(it);
			return;
		}
	}
};

Settler* Resources::GetIdleSettler()
{
	Settler* last = nullptr;
	if (idleSettlers.size() != 0)
	{
		last = idleSettlers.back();
		idleSettlers.pop_back();
	}
	return last;
};

Building* Resources::GetIdleBuilding()
{
	Building* last = nullptr;
	if (idleBuildings.size() != 0)
	{
		last = idleBuildings.back();
		idleBuildings.pop_back();
	}
	return last;
};

// returns empty vector if not enough idle settlers found
std::vector<Settler*> Resources::GetIdleSettlers(int nofSettlersNeeded)
{
	std::vector<Settler*> settlers = std::vector<Settler*>();

	if (idleSettlers.size() >= nofSettlersNeeded)
	{
		for (int i = 0; i < nofSettlersNeeded; ++i)
			settlers.push_back(GetIdleSettler());
	}
	return settlers;
};
