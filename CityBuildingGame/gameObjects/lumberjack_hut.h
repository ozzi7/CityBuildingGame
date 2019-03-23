#pragma once
#include <building.h>
#include "visitor.h"

class Grid;

class LumberjackHut : public Building
{
public:
	LumberjackHut(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation);
	void Accept(Visitor &v);
private:

};