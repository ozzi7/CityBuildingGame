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
	if (hasArrived)
	{  
		if (state == returningHome) {
			// arrived back home..
			// TODO: add resources
			// find new tree.. 
			unitEventHandler->AddEvent(new GatherResourceEvent(Wood, this));
		}
		else if (state == walkingToTarget)
		{
			// arrived at tree, start working.. 
			state = working;
			workTimeLeft = 10000;
		}
	}
	else if (state == working && workTimeLeft == 0) {
		// done working.. delete tree, go home
		unitEventHandler->AddEvent(new DeleteEvent(destination->posX, destination->posY, destination));
		unitEventHandler->AddEvent(new ReturnHomeEvent(this, LumberjackID));
		state = returningHome;
	}
	else if (state == working)
	{
		workTimeLeft--;
	}

	/*Update animation*/
	animationSecond += walkingSpeed * 1.80f; // 100fps?
}
