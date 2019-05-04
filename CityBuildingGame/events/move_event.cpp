#include "stdafx.h"
#pragma once
#include "move_event.h"

MoveEvent::MoveEvent(int aFromX, int aFromY, int aToX, int aToY, GameObject* aGameObject)
{
	fromX = aFromX;
	fromY = aFromY;

	toX = aToX;
	toY = aToY;

	gameObject = aGameObject;
};
void MoveEvent::Accept(GameEventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
};