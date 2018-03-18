#pragma once
#include "bone_animated.h"
#include "visitor.h"

class Lumberjack : public BoneAnimated
{
public:
	Lumberjack(glm::vec3 aPosition, glm::vec3 aScale, float aRotation)
		: BoneAnimated(aPosition, aScale, aRotation) {};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};
};