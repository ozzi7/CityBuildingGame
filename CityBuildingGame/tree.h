#pragma once
#include <glm/glm.hpp>

class Tree
{
	public:
		glm::vec3 Position;

		Tree(glm::vec3);
	void ChangeSize();
};