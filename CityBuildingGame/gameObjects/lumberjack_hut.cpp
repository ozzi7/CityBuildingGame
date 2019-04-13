#include "stdafx.h"
#pragma once
#include <lumberjack_hut.h>


LumberjackHut::LumberjackHut(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: Building(aPosition, aScale, aRotation) {
};
void LumberjackHut::Accept(Visitor &v)
{
	v.Visit(this);
};

void LumberjackHut::Evolve()
{
	evolutionStage++;

	scale = glm::vec3(0.014f, 0.014f, 0.014f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	recalculateModelMatix();
}
