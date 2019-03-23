#include "stdafx.h"
#pragma once
#include "delete_event.h"

DeleteEvent::DeleteEvent(int aPosX, int aPosY, GameObject * aGameObject)
{
	posX = aPosX;
	posY = aPosY;

	gameObject = aGameObject;
};
void DeleteEvent::Accept(EventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
};