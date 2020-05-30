#pragma once
#include <glm/glm.hpp>
#include "game_object.h"
#include "visitor.h"
#include <math.h>
#include <vector>

class Building : public GameObject
{
public:
	Building::Building(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation, float aFloorZ);

	/* the building covers [fromX, toX] x [fromY, toY] */
	int sizeX = 0;
	int sizeY = 0;

	int fromX = 0;
	int fromY = 0;
	int toX = 0;
	int toY = 0;

	int entranceX = 0;
	int entranceY = 0;
	
	float floorZ = 0.0f;

	/* 0 when created, 1 when done constructing (used for 3d model selection) */
	unsigned short evolutionStage = 0;
	std::vector<glm::mat4> buildingOutlines;
	std::vector<glm::mat4> woodModels;

	void CreateBuildingOutline();
	virtual void Evolve() = 0;
	virtual void UpdateWoodModels() = 0;

	// When dropping off wood
	void AddWoodBuildingMaterial(unsigned int wood = 1);
	// When dropping off stone
	void AddStoneBuildingMaterial(unsigned int stone = 1);
	// When worker arrives
	void AddWorker(unsigned int workers = 1);

	// Reserve wood that will eventually arrive
	void AddWoodBuildingMaterialOnTheWay(unsigned int wood = 1);
	// Reserve stone that will eventually arrive
	void AddStoneBuildingMaterialOnTheWay(unsigned int stone = 1);
	// Reserve worker that will eventually arrive 
	void AddWorkerOnTheWay(unsigned int workers = 1);

	// Reserve wood that is needed somewhere else
	void ReserveWoodBuildingMaterial(unsigned int wood = 1);
	// Reserve stone that is needed somewhere else
	void ReserveStoneBuildingMaterial(unsigned int stone = 1);

	// When removing wood from building
	bool RemoveWoodBuildingMaterial(unsigned int wood = 1);
	// When removing stone from building
	bool RemoveStoneBuildingMaterial(unsigned int stone = 1);
	// When removing worker from building
	bool RemoveWorker(unsigned int workers = 1);

	// Uses all available building materials
	void ConsumeAllBuildingMaterial();

	// Wood still required after everything in transit has arrived
	int WoodBuildingMaterialRequired() const;
	// Stone still required after everything in transit has arrived
	int StoneBuildingMaterialRequired() const;
	// Workers still required after everyone in transit has arrived
	int WorkersRequired() const;

	// Stored wood that is not reserved
	int UnusedWoodBuildingMaterial() const;
	// Stored stone that is not reserved
	int UnusedStoneBuildingMaterial() const;
	// Workers onsite that are not required
	int UnusedWorkers() const;

	// All required building materials have arrived, excl. workers
	bool AllRequiredBuildingMaterialsOnSite() const;
	// All required building materials will eventually arrive, excl. workers
	bool AllRequiredBuildingMaterialsOnTheWay() const;
	// All required workers have arrived
	bool AllRequiredWorkersOnSite() const;
	// All required workers will eventually arrive
	bool AllRequiredWorkersOnTheWay() const;

protected:
	
	/*building requirements needed & delivered.. */
	unsigned int workersPresent = 0;
	unsigned int workersRequired = 0;
	unsigned int workersOnTheWay = 0;

	unsigned int woodStored = 0;
	unsigned int woodRequired = 0;
	unsigned int woodOnTheWay = 0;
	unsigned int woodReserved = 0;

	unsigned int stoneStored = 0;
	unsigned int stoneRequired = 0;
	unsigned int stoneOnTheWay = 0;
	unsigned int stoneReserved = 0;
};
