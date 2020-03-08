#pragma once
#include <list>
#include <vector>

class Settler;
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
	void AddIdleSettler(Settler* idleSettler);
	void AddIdleBuilding(Building* idleBuilding);

	void AddLumberjack(Lumberjack* lumberjack);
	void AddSettler(Settler* settler);
	void AddDwelling(Dwelling* dwelling);
	void AddLumberjackHut(LumberjackHut* lumberjackHut);
	void RemoveObject(GameObject* gameObject); 

	Settler* GetIdleSettler();
	Building* GetIdleBuilding();
	std::vector<Settler*> GetIdleSettlers(int nofSettlersNeeded);

	std::list<Settler*> idleSettlers = std::list<Settler*>();
	std::list<Building*> idleBuildings = std::list<Building*>();

	std::list<Lumberjack*> lumberjacks = std::list<Lumberjack*>();
	std::list<Settler*> settlers = std::list<Settler*>();
	std::list<Dwelling*> dwellings = std::list<Dwelling*>();
	std::list<LumberjackHut*> lumberjackHuts = std::list<LumberjackHut*>();
};
