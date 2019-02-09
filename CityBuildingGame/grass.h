#pragma once
#include "tree.h"
#include "visitor.h"

class Grass : public Tree
{
public:
	Grass(glm::vec3 aPosition, glm::vec3 aScale, float aRotation)
		: Tree(aPosition, aScale, aRotation) {
		model = glm::translate(model, position);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.18f));

		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::scale(model, scale);
		model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};
};