#pragma once
#include <glm/glm.hpp>
#include "object.h"
#include "visitor.h"

class Tree : public Object
{
	public:
		Tree(glm::vec3 aPosition, glm::vec3 aScale, float aRotation)
			: Object(aPosition, aScale, aRotation) {};
};