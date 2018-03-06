#pragma once
#include "tree.h"
#include "visitor.h"

class Fir: public Tree
{
public:
	Fir(glm::vec3 position) : Tree(position) {};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};
};