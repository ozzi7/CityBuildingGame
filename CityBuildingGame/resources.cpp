#include "stdafx.h"
#pragma once

#include "resources.h"

Resources::Resources()
{
}
Resources::~Resources()
{
}
void Resources::AddIdleSettler(Settler* idleSettler)
{
	Resources::idleSettlers.push_back(idleSettler);
};
void Resources::AddIdleBuilding(Building* idleBuilding)
{
	Resources::idleBuildings.push_back(idleBuilding);
};
Settler* Resources::GetIdleSettler()
{
	Settler* last = NULL;
	if (idleSettlers.size() != 0) {
		last = idleSettlers.back();
		idleSettlers.pop_back();
	}
	return last;
};
Building* Resources::GetIdleBuilding()
{
	Building* last = NULL;
	if (idleBuildings.size() != 0) {
		last = idleBuildings.back();
		idleBuildings.pop_back();
	}
	return last;
};
std::vector<Settler*> Resources::GetIdleSettlers(int nofSettlersNeeded)
{
	std::vector<Settler*> settlers = std::vector<Settler*>();

	if (idleSettlers.size() >= nofSettlersNeeded) {
		for (int i = 0; i < nofSettlersNeeded; ++i)
			settlers.push_back(GetIdleSettler());
	}
	return settlers;
};