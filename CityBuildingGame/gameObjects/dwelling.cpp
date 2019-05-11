#include "stdafx.h"
#pragma once
#include <dwelling.h>

Dwelling::Dwelling(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: Building(aPosition, aScale, aRotation) {};

void Dwelling::Accept(Visitor& v)
{
	v.Visit(this);
};

void Dwelling::Evolve()
{
	evolutionStage++;

	scale = glm::vec3(0.14f, 0.14f, 0.14f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	recalculateModelMatix();
}
