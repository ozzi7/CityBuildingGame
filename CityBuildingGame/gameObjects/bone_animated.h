#pragma once
#include <gameObjects\object.h>

class BoneAnimated : public Object
{
public:
	BoneAnimated(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: Object(aPosition, aScale, aRotation) {};

	virtual void UpdatePosition() = 0;
};