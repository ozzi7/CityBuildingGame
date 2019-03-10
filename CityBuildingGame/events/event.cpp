#include "stdafx.h"
#pragma once
#include "event.h"

Event::Event() {
	// for now this event is to create objects, move objects, delete objects
}
Event::Event(eventType MoveObject, int aFromX, int aFromY, int aToX, int aToY, int aIndex)
{
	fromX = aFromX;
	fromY = aFromY;
	toX = aToX;
	toY = aToY;
	index = aIndex;
};