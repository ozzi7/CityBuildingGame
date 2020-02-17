#pragma once
#include "tree.h"
#include "visitor.h"

class Toona : public Tree
{
public:
	Toona(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: Tree(aPosition, aScale, aRotation) { };

	void Accept(Visitor& v) override
	{
		v.Visit(this);
	};
};
