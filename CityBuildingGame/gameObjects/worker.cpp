#include "stdafx.h"
#pragma once
#include <worker.h>
#include <dwelling.h>

Worker::Worker(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: BoneAnimated(aPosition, aScale * 0.35f, aRotation) {};

void Worker::Accept(Visitor& v)
{
	v.Visit(this);
};

void Worker::SetDwelling(Dwelling* aDwelling)
{
	dwelling = aDwelling;
}

void Worker::GameStep()
{
	/* check if arrived at dwelling*/
	if (hasArrived && dwelling->evolutionStage == 0)
	{
		dwelling->Evolve();
		soundEventHandler->AddEvent(new PlaySoundEvent(SoundType::WorkerArrivedSound));
		visible = false;
		state = State::idle;
		resources->AddIdleWorker(this);
	}
	else
	{
		/*Update animation*/
		animationSecond += walkingSpeed * 1.80f; // 100fps?
	}
}
