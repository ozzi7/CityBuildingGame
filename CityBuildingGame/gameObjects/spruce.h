#pragma once
#include "tree.h"
#include "visitor.h"

class Spruce : public Tree
{
public:
	Spruce(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: Tree(aPosition, aScale, aRotation) { };

	void Accept(Visitor& v) override
	{
		v.Visit(this);
	};
};
