#pragma once
#include "tree.h"
#include "visitor.h"

class Fir: public Tree
{
public:
	Fir(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation) 
		: Tree(aPosition, aScale, aRotation) {
	};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};
};