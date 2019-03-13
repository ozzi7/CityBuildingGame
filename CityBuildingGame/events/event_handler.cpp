#include "stdafx.h"
#pragma once
#include <event_handler.h>
#include <bone_animated.h>
#include <dwelling.h>
#include <bone_animated.h>
#include <grid.h>

EventHandler::EventHandler(Grid * aGrid)
{
	grid = aGrid;
}
void EventHandler::AddEvent(Event * e)
{
	cq.enqueue(e);
}
Event * EventHandler::GetEvent()
{
	Event * e;
	if (cq.try_dequeue(e))
	{
		return e;
	}
	else
	{
		return NULL; // ??
	}
}
bool EventHandler::ProcessEvent()
{
	Event * game_event = GetEvent();
	if (game_event != NULL)
	{
		game_event->Accept(this);
		return true;
	}
	return false;
}
void EventHandler::Visit(MoveEvent * aMoveEvent)
{
	/* removes element by index, could use pointers instead but... */
	int count = 0;
	BoneAnimated * toMove = NULL;
	for (auto it = grid->gridUnits[aMoveEvent->fromY][aMoveEvent->fromX]->movingObjects.begin(); it !=
		grid->gridUnits[aMoveEvent->fromY][aMoveEvent->fromX]->movingObjects.end(); ++it) {
		if (count == aMoveEvent->index) {
			toMove = (*it);
			grid->gridUnits[aMoveEvent->fromY][aMoveEvent->fromX]->movingObjects.erase(it);
			break;
		}
		count++;
	}
	grid->gridUnits[aMoveEvent->toY][aMoveEvent->toX]->movingObjects.push_back(toMove);
}
void EventHandler::Visit(CreateBuildingEvent * aCreateBuildingEvent)
{
	if (aCreateBuildingEvent->posX + 1 < grid->gridWidth && aCreateBuildingEvent->posY + 1 < grid->gridHeight) {
		if (grid->gridUnits[aCreateBuildingEvent->posY][aCreateBuildingEvent->posX]->occupied == false &&
			grid->gridUnits[aCreateBuildingEvent->posY + 1][aCreateBuildingEvent->posX]->occupied == false &&
			grid->gridUnits[aCreateBuildingEvent->posY + 1][aCreateBuildingEvent->posX + 1]->occupied == false &&
			grid->gridUnits[aCreateBuildingEvent->posY][aCreateBuildingEvent->posX + 1]->occupied == false) {
			grid->gridUnits[aCreateBuildingEvent->posY][aCreateBuildingEvent->posX]->objects.push_back(new Dwelling(glm::vec3(aCreateBuildingEvent->posX + 1.0f, aCreateBuildingEvent->posY + 1.0f,
				grid->GetHeight(aCreateBuildingEvent->posX + 1.0f, aCreateBuildingEvent->posY + 1.0f)),
				glm::vec3(0.14f, 0.14f, 0.14f),
				glm::vec3(0.0f, 0.0f, 0.0f)));
			grid->gridUnits[aCreateBuildingEvent->posY][aCreateBuildingEvent->posX]->occupied = true;
			grid->gridUnits[aCreateBuildingEvent->posY + 1][aCreateBuildingEvent->posX + 1]->occupied = true;
			grid->gridUnits[aCreateBuildingEvent->posY + 1][aCreateBuildingEvent->posX + 1]->occupied = true;
			grid->gridUnits[aCreateBuildingEvent->posY][aCreateBuildingEvent->posX + 1]->occupied = true;
		}
	}
}