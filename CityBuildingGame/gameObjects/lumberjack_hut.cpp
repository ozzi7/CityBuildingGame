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

	/* Exchange 3d model scale, rotation */
}
