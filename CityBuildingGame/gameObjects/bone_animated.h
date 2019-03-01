#pragma once
#include <game_object.h>

class BoneAnimated : public GameObject
{
public:
	RenderObjType type = boneAnimated;
	BoneAnimated(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: GameObject(aPosition, aScale, aRotation) {};

	virtual void UpdatePosition() = 0;
};