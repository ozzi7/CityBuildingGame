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
			lumberjackHut->wood++;
			lumberjackHut->UpdateWoodModels();
			if (lumberjackHut->evolutionStage == 0)
			{
				lumberjackHut->Evolve();
			}
			if (lumberjackHut->wood < lumberjackHut->woodCapacity) {
				// find new tree if the hut can accept more wood...
				unitEventHandler->AddEvent(new GatherResourceEvent(Wood, this));
			}
			else
			{
				state = State::idle;
				visible = false;
			}
		}
		else if (state == State::walkingToTarget)
		{
			// arrived at tree, start working..
			state = State::working;
			workTimeLeft = 1000;
			animationSecond = 0.0f;
		}
		// TODO: check the idle workers here too? instead of doing it in returnhome above.. 
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

		if (fmod(animationSecond, 1.0f) < (16.0f/30.0f))
			soundPlayed = false;
		if (fmod(animationSecond, 1.0f) >= (16.0f/30.0f) && !soundPlayed) {
			soundEventHandler->AddEvent(new PlaySoundEvent(SoundType::TreeChoppingSound));
			soundPlayed = true;
		}
	}

	/*Update animation*/
	animationSecond += walkingSpeed * 1.80f; // 100fps?
}
