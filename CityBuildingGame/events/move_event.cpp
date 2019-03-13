#include "stdafx.h"
#pragma once
#include "move_event.h"

MoveEvent::MoveEvent(int aFromX, int aFromY, int aToX, int aToY, int aIndex)
{
	fromX = aFromX;
	fromY = aFromY;

	toX = aToX;
	toY = aToY;

	index = aIndex;
};
void MoveEvent::Accept(EventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
};