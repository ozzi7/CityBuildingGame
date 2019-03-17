#include "stdafx.h"
#pragma once
#include <lumberjack.h>

Lumberjack::Lumberjack(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: BoneAnimated(aPosition, aScale, aRotation) {
};
void Lumberjack::Accept(Visitor &v)
{
	v.Visit(this);
};