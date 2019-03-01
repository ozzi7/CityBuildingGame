#pragma once
#include <game_object.h>
#include "visitor.h"

class BoneAnimated : public GameObject
{
public:
	BoneAnimated(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: GameObject(aPosition, aScale, aRotation) {};

	virtual void UpdatePosition() = 0;
};