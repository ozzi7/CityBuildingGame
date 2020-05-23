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
	if (hasArrived && state == State::immigrating && dwelling->evolutionStage == 0)
	{
		dwelling->Evolve();
		dwelling->woodStored = dwelling->woodStored + 2;
		soundEventHandler->AddEvent(new PlaySoundEvent(SoundType::WorkerArrivedSound));
		visible = false;
		state = State::idle;
		resources->AddIdleWorker(this);
	}
	else if (hasArrived && state == State::carryingWood) {
		resourceTargetBuilding->AddWoodBuildingMaterial();
		state = State::idle;
		resources->AddIdleWorker(this);
		gameEventHandler->AddEvent(new ResourceArrivedEvent(Resource::Wood, resourceTargetBuilding));
	}
	else if (hasArrived && state == State::carryingStone)
	{
		resourceTargetBuilding->AddStoneBuildingMaterial();
		state = State::idle;
		resources->AddIdleWorker(this);
		gameEventHandler->AddEvent(new ResourceArrivedEvent(Resource::Stone, resourceTargetBuilding));
	}
	else if (hasArrived && state == State::goingToWork)
	{
		gameEventHandler->AddEvent(new WorkerArrivedEvent(destination));
		gameEventHandler->AddEvent(new DeleteEvent(posX, posY, this));
	}
	else if (hasArrived && state == State::gettingWood || state == State::gettingStone)
	{
		state = State::pending;
		gameEventHandler->AddEvent(new BringResourceEvent(this, 0));
	}
	else
	{
		/*Update animation*/
		animationSecond += walkingSpeed * 1.80f; // 100fps?
	}
}
