#pragma once
#include "tree.h"
#include "visitor.h"

class WhiteTree : public Tree
{
public:
	WhiteTree(glm::vec3 position) : Tree(position) 
	{
	};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};
};