#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "visitor.h"

class Object
{
public:
	Object();
	Object(glm::vec3 aPosition, glm::vec3 aScale, float aRotation);

	glm::vec3 position;
	glm::vec3 scale;
	float rotation;
	glm::mat4 model = glm::mat4(1.0f);

	virtual void Accept(class Visitor &v) = 0;
};