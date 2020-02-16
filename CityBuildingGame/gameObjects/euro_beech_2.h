#pragma once
#include "tree.h"
#include "visitor.h"

class EuroBeech2 : public Tree
{
public:
	EuroBeech2(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: Tree(aPosition, aScale, aRotation) { };

	void Accept(Visitor& v) override
	{
		v.Visit(this);
	};
};
