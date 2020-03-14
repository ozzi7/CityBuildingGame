#pragma once
#include <building.h>
#include "visitor.h"

class Grid;

class LumberjackHut : public Building
{
public:
	LumberjackHut(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation);
	void Accept(Visitor& v) override;
	void Evolve() override;
	void UpdateWoodModels();

	int wood = 0;
	const int woodCapacity = 9;

private:
};
