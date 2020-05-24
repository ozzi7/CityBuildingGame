#pragma once
#include <building.h>
#include "visitor.h"

class Grid;

class Dwelling : public Building
{
public:
	Dwelling(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation);
	void Accept(Visitor& v) override;
	void Evolve() override;
	void UpdateWoodModels();

private:
};
