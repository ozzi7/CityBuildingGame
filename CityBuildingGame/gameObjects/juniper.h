#pragma once
#include "tree.h"
#include "visitor.h"

class Juniper : public Tree
{
public:
	Juniper(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: Tree(aPosition, aScale, aRotation) { };

	void Accept(Visitor& v) override
	{
		v.Visit(this);
	};
};
