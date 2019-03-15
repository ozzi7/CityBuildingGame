#include "stdafx.h"
#pragma once
#include <event_handler.h>
#include <grid.h>
#include <lumberjack.h>
#include <bone_animated.h>
#include <dwelling.h>

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
		return NULL;
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
	BoneAnimated * toMove = NULL;
	for (auto it = grid->gridUnits[aMoveEvent->fromY][aMoveEvent->fromX]->movingObjects.begin(); it !=
		grid->gridUnits[aMoveEvent->fromY][aMoveEvent->fromX]->movingObjects.end(); ++it) {
		if ((*it) == aMoveEvent->gameObject) {
			toMove = (*it);
			grid->gridUnits[aMoveEvent->fromY][aMoveEvent->fromX]->movingObjects.erase(it);
			break;
		}
	}
	grid->gridUnits[aMoveEvent->toY][aMoveEvent->toX]->movingObjects.push_back(toMove);
}
void EventHandler::Visit(CreateBuildingEvent * aCreateBuildingEvent)
{
	int buildingCenterX = round(aCreateBuildingEvent->posX);
	int buildingCenterY = round(aCreateBuildingEvent->posY);

	switch (aCreateBuildingEvent->buildingType) {
		case LumberjackHutID:
		{
			bool success = false;

			// Dwelling 2x2
			if (buildingCenterX < grid->gridWidth && buildingCenterY < grid->gridHeight &&
				buildingCenterX - 1 >= 0 && buildingCenterY - 1 >= 0) {
				if (!grid->gridUnits[buildingCenterY - 1][buildingCenterX - 1]->occupied &&
					!grid->gridUnits[buildingCenterY - 1][buildingCenterX]->occupied &&
					!grid->gridUnits[buildingCenterY][buildingCenterX]->occupied &&
					!grid->gridUnits[buildingCenterY][buildingCenterX - 1]->occupied) {
					if (grid->IsAreaFlat(buildingCenterX - 1, buildingCenterX, buildingCenterY - 1, buildingCenterY))
					{
						/* save building in the "middle" unit -> best for rendering */
						grid->gridUnits[buildingCenterY][buildingCenterX]->objects.push_back(
							new Dwelling(glm::vec3(buildingCenterX, buildingCenterY,
								grid->GetHeight(buildingCenterX, buildingCenterY)),
								glm::vec3(0.14f, 0.14f, 0.14f),
								glm::vec3(0.0f, 0.0f, 0.0f)));
						grid->gridUnits[buildingCenterY - 1][buildingCenterX - 1]->occupied = true;
						grid->gridUnits[buildingCenterY - 1][buildingCenterX]->occupied = true;
						grid->gridUnits[buildingCenterY][buildingCenterX]->occupied = true;
						grid->gridUnits[buildingCenterY][buildingCenterX - 1]->occupied = true;
						success = true;
					}
					else {
						// the area is not completely flat
					}
				}
				else
				{
					// occupied grid units
				}
			}
			else {
				// building outside the grid
			}
			if (success)
			{
				// then also create a lumberjack
				grid->gridUnits[2][2]->movingObjects.push_back(new Lumberjack(glm::vec3(2 + 0.5f, 2 + 0.5f, grid->gridUnits[2][2]->averageHeight),
					glm::vec3(0.0045f, 0.0045f, 0.0045f), glm::vec3(0, 0, glm::pi<float>())));

			}
		}
		break;

		case DwellingID:
			// Dwelling 2x2
			if (buildingCenterX < grid->gridWidth && buildingCenterY < grid->gridHeight &&
				buildingCenterX - 1 >= 0 && buildingCenterY - 1 >= 0) {
				if (!grid->gridUnits[buildingCenterY - 1][buildingCenterX - 1]->occupied &&
					!grid->gridUnits[buildingCenterY - 1][buildingCenterX]->occupied &&
					!grid->gridUnits[buildingCenterY][buildingCenterX]->occupied &&
					!grid->gridUnits[buildingCenterY][buildingCenterX - 1]->occupied) {
					if (grid->IsAreaFlat(buildingCenterX - 1, buildingCenterX, buildingCenterY - 1, buildingCenterY))
					{
						/* save building in the "middle" unit -> best for rendering */
						grid->gridUnits[buildingCenterY][buildingCenterX]->objects.push_back(
							new Dwelling(glm::vec3(buildingCenterX, buildingCenterY,
								grid->GetHeight(buildingCenterX, buildingCenterY)),
								glm::vec3(0.14f, 0.14f, 0.14f),
								glm::vec3(0.0f, 0.0f, 0.0f)));
						grid->gridUnits[buildingCenterY - 1][buildingCenterX - 1]->occupied = true;
						grid->gridUnits[buildingCenterY - 1][buildingCenterX]->occupied = true;
						grid->gridUnits[buildingCenterY][buildingCenterX]->occupied = true;
						grid->gridUnits[buildingCenterY][buildingCenterX - 1]->occupied = true;
					}
					else {
						// the area is not completely flat
					}
				}
				else
				{
					// occupied grid units
				}
			}
			else {
				// building outside the grid
			}
				
			break;
	}	
}