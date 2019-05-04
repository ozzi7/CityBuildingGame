#include "stdafx.h"
#pragma once
#include <settler.h>
#include <dwelling.h>

Settler::Settler(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: BoneAnimated(aPosition, aScale * 0.35f, aRotation) {
};
void Settler::Accept(Visitor& v)
{
	v.Visit(this);
};

void Settler::SetDwelling(Dwelling* aDwelling) {
	dwelling = aDwelling;
}
void Settler::GameStep() {
	/* check if arrived at dwelling*/
	if (hasArrived && dwelling->evolutionStage == 0) {
		dwelling->Evolve();
		soundEventHandler->AddEvent(new PlaySoundEvent(SettlerArrivedID));
		visible = false;
		state = idle;
		resources->AddIdleSettler(this);
	}
	else {
		/*Update animation*/
		animationSecond += walkingSpeed * 1.80f; // 100fps?
	}
}