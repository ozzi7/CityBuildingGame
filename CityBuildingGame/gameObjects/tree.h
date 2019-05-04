#pragma once
#include <glm/glm.hpp>
#include "game_object.h"
#include "visitor.h"

class Tree : public GameObject
{
public:
	Tree(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: GameObject(aPosition, aScale, aRotation) {};
	void Accept(Visitor& v)
	{
		v.Visit(this);
	};
};