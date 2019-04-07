#include "stdafx.h"
#pragma once
#include <settler.h>
#include <dwelling.h>

Settler::Settler(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: BoneAnimated(aPosition, aScale, aRotation) {
};
void Settler::Accept(Visitor &v)
{
	v.Visit(this);
};

void Settler::SetDwelling(Dwelling * aDwelling) {
	dwelling = aDwelling;
}
void Settler::GameStep() {
	/* check if arrived at dwelling*/
	if (hasArrived != updatedDwelling) {
		dwelling->Evolve();
		updatedDwelling = true;
		unitEventHandler->AddEvent(new DeleteEvent(posX, posY, this));
	}

	/*Update animation*/
	animationSecond += walkingSpeed*1.80f; // 100fps?
}