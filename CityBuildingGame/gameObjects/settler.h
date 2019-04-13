#pragma once
#include "bone_animated.h"
#include <math.h> 
#include "visitor.h"
#include <grid.h>

class Settler : public BoneAnimated
{
public:
	Settler(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation);
	void Accept(Visitor &v);
	void SetDwelling(Dwelling * aDwelling);
	void GameStep();

protected:
	Dwelling * dwelling;
};