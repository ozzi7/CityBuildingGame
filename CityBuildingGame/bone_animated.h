#pragma once
#include "visitor.h"

class BoneAnimated : public Object
{
public:
	BoneAnimated(glm::vec3 aPosition, glm::vec3 aScale, float aRotation)
		: Object(aPosition, aScale, aRotation) {};

	virtual void UpdatePosition() = 0;
};