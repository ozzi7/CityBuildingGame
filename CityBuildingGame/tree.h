#pragma once
#include <glm/glm.hpp>
#include "object.h"
#include "visitor.h"

class Tree : public Object
{
	public:
		Tree(glm::vec3 aPosition, glm::vec3 aScale, float aRotation);

		glm::vec3 position;
		glm::vec3 scale;
		float rotation;

		void Accept(Visitor &v);
};