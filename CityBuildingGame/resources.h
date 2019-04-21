#pragma once
#include <vector>

class Settler;
class Building;

class Resources
{
public:
	Resources();
	~Resources();
	void AddIdleSettler(Settler* idleSettler);
	void Resources::AddIdleBuilding(Building* idleBuilding);

	Settler* GetIdleSettler();
	Building* GetIdleBuilding();
	std::vector<Settler*> Resources::GetIdleSettlers(int nofSettlersNeeded);

	std::vector<Settler*> idleSettlers = std::vector<Settler*>();
	std::vector<Building*> idleBuildings = std::vector<Building*>();
};
