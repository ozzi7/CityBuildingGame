#pragma once
#include "object.h"

Object::Object(glm::vec3 aPosition, glm::vec3 aScale, float aRotation)
{
	position = aPosition;
	scale = aScale;
	rotation = aRotation;
}
Object::Object() {};