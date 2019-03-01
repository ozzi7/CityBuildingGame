#pragma once
#include <glm/glm.hpp>
#include "game_object.h"

class Tree : public GameObject
{
	public:
		RenderObjType type = tree;

		Tree(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
			: GameObject(aPosition, aScale, aRotation) {};
};