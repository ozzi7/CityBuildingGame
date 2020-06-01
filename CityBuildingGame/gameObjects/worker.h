#pragma once
#include "bone_animated.h"
#include <math.h>
#include "visitor.h"
#include <grid.h>

class Worker : public BoneAnimated
{
public:
	Worker(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation);
	void Accept(Visitor& v) override;
	void GameStep() override;

	Building* resourceTargetBuilding{};
	bool goingHome = false;
	Dwelling* dwelling;

};
