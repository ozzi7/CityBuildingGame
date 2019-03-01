#pragma once
#include "tree.h"

class Grass : public Tree
{
public:
	RenderObjType type = grass;

	Grass(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: Tree(aPosition, aScale, aRotation) {
		//model = glm::translate(model, position);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.18f));

		//model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		//model = glm::scale(model, scale);
		//model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	};
};