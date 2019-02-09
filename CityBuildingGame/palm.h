#pragma once
#include "tree.h"
#include "visitor.h"

class Palm: public Tree
{
public:
	Palm(glm::vec3 aPosition, glm::vec3 aScale, float aRotation)
		: Tree(aPosition, aScale, aRotation) {
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05));
	};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};
};