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

void Worker::GameStep()
{
	/* check if arrived at dwelling*/
	if (hasArrived && state == State::immigrating && dwelling->evolutionStage == 0)
	{
		dwelling->AddWoodBuildingMaterial(2);
		soundEventHandler->AddEvent(new PlaySoundEvent(SoundType::WorkerArrivedSound));
		visible = false;
		gameEventHandler->SetWorkerStateIdle(this);
	}
	else if (hasArrived && state == State::carryingWood) {
		resourceTargetBuilding->AddWoodBuildingMaterial();
		resourceTargetBuilding->UpdateWoodModels();
		gameEventHandler->SetWorkerStateIdle(this);
		gameEventHandler->AddEvent(new ResourceArrivedEvent(Resource::Wood, resourceTargetBuilding));
	}
	else if (hasArrived && state == State::carryingStone)
	{
		resourceTargetBuilding->AddStoneBuildingMaterial();
		gameEventHandler->SetWorkerStateIdle(this);
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
	else if (hasArrived && goingHome)
	{
		dwelling->AddWorker();
		visible = false;
		goingHome = false;
	}
	else
	{
		/*Update animation*/
		animationSecond += walkingSpeed * 1.80f; // 100fps?
	}
}
