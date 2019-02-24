#pragma once
#include <glm/glm.hpp>
#include "object.h"

class Tree : public Object
{
	public:
		Tree(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
			: Object(aPosition, aScale, aRotation) {};
};