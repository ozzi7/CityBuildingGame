#pragma once
#include "tree.h"
#include "visitor.h"

class Chamaecyparis : public Tree
{
public:
	Chamaecyparis(glm::vec3 position) : Tree(position)
	{
	};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};
};