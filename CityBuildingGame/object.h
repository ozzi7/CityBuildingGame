#pragma once

#include <glm/glm.hpp>
#include "visitor.h"

class Object
{
public:
	Object();
	Object(glm::vec3 aPosition, glm::vec3 aScale, float aRotation);

	glm::vec3 position;
	glm::vec3 scale;
	float rotation;

	virtual void Accept(class Visitor &v) = 0;
};