#pragma once
#include <building.h>
#include "visitor.h"

class Grid;

class Dwelling : public Building
{
public:
	Dwelling::Dwelling(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation, float aFloorZ);
	void Accept(Visitor& v) override;
	void Evolve() override;
	void UpdateWoodModels() override;

	// When worker arrives
	void AddWorker(unsigned int workers = 1) override;
	
	// Has capacity to store a worker
	bool FreeWorkerCapacity() const;

private:
	int workersOnTheWay = 0;
	int workersPresent = 0;
	int workerCapacity = 1;
};
