#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class Object
{
public:
	Object();
	Object(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation);

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::mat4 model = glm::mat4(1.0f);

protected:
	void recalculateModelMatix();
};