#pragma once
#include "bone_animated.h"
#include <math.h> 
#include "visitor.h"
#include <grid.h>

class Lumberjack : public BoneAnimated
{
public:
	Lumberjack(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation);
	
	void Accept(Visitor &v);

private:
	float z = 0.0f;
	int wayPointIndex = 0;
	float walkingSpeed = 0.005f; // TODO: depends on model size.. 
	float maxRotation = 0.1f;

	bool hasArrived();
};