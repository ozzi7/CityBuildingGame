#pragma once
#include <glm/glm.hpp>
#include "object.h"
#include "visitor.h"

class Tree : public Object
{
	public:
		Tree(glm::vec3 position);
		glm::vec3 Position;

		void Accept(Visitor &v);
};