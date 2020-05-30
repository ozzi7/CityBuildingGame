#pragma once
#include <building.h>
#include "visitor.h"

class Grid;

class LumberjackHut : public Building
{
public:
	LumberjackHut::LumberjackHut(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation, float aFloorZ);

	void Accept(Visitor& v) override;
	void Evolve() override;
	void UpdateWoodModels() override;

	int wood = 0;
	const int woodCapacity = 10;

private:
};
