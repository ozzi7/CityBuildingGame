#include "stdafx.h"
#pragma once
#include <lumberjack.h>
#include <lumberjack_hut.h>

Lumberjack::Lumberjack(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: BoneAnimated(aPosition, aScale * 0.35f, aRotation) {};

void Lumberjack::Accept(Visitor& v)
{
	v.Visit(this);
};

void Lumberjack::SetLumberjackHut(LumberjackHut* aLumberjackHut)
{
	lumberjackHut = aLumberjackHut;
}

void Lumberjack::GameStep()
{
	if (hasArrived)
	{
		if (state == State::returningHome)
		{
			// arrived back home..
			// TODO: add resources
			// find new tree..
			if (lumberjackHut->evolutionStage == 0)
			{
				lumberjackHut->Evolve();
			}
			unitEventHandler->AddEvent(new GatherResourceEvent(Wood, this));
		}
		else if (state == State::walkingToTarget)
		{
			// arrived at tree, start working..
			state = State::working;
			workTimeLeft = 1000;
		}
	}
	if (state == State::working && workTimeLeft == 0)
	{
		// done working.. delete tree, go home
		unitEventHandler->AddEvent(new DeleteEvent(destination->posX, destination->posY, destination));
		unitEventHandler->AddEvent(new ReturnHomeEvent(this, PersonType::LumberjackID));
		state = State::returningHome;
	}
	else if (state == State::working)
	{
		workTimeLeft--;
	}

	/*Update animation*/
	animationSecond += walkingSpeed * 1.80f; // 100fps?
}
