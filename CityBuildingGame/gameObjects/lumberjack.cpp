#include "stdafx.h"
#pragma once
#include <lumberjack.h>
#include <lumberjack_hut.h>

Lumberjack::Lumberjack(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: BoneAnimated(aPosition, aScale, aRotation) {
};
void Lumberjack::Accept(Visitor &v)
{
	v.Visit(this);
};

void Lumberjack::SetLumberjackHut(LumberjackHut * aLumberjackHut) {
	lumberjackHut = aLumberjackHut;
}
void Lumberjack::GameStep() {
	if (state == idle) {
		if (destination)
			unitEventHandler->AddEvent(new DeleteEvent(destination->posX, destination->posY, destination));

		unitEventHandler->AddEvent(new GatherResourceEvent(Wood, this));
	}

	/*Update animation*/
	animationSecond += walkingSpeed * 1.80f; // 100fps?
}