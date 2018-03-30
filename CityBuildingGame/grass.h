#pragma once
#include "tree.h"
#include "visitor.h"

class Grass : public Tree
{
public:
	Grass(glm::vec3 aPosition, glm::vec3 aScale, float aRotation)
		: Tree(aPosition, aScale, aRotation) {};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};
};