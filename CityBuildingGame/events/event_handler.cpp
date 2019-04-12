#include "stdafx.h"
#pragma once
#include <event_handler.h>
#include <grid.h>
#include <bone_animated.h>
#include <dwelling.h>
#include <lumberjack.h>
#include <settler.h>
#include <lumberjack_hut.h>
#include "pathfinding.h"
#include "pathfinding_object.h"

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
	/* removes element found by reference */
	BoneAnimated * toMove = NULL;
	for (auto it = grid->gridUnits[aMoveEvent->fromY][aMoveEvent->fromX]->movingObjects.begin(); it !=
		grid->gridUnits[aMoveEvent->fromY][aMoveEvent->fromX]->movingObjects.end(); ++it) {
		if ((*it) == aMoveEvent->gameObject) {
			toMove = *it;
			it = grid->gridUnits[aMoveEvent->fromY][aMoveEvent->fromX]->movingObjects.erase(it);
			break;
		}
	}
	if (toMove == NULL)
		// should not happen
		bool problem = true;
	else
		grid->gridUnits[aMoveEvent->toY][aMoveEvent->toX]->movingObjects.push_back(toMove);
}
void EventHandler::Visit(CreateBuildingEvent * aCreateBuildingEvent)
{
	std::tuple<int, int> closestToClick = std::make_tuple(round(aCreateBuildingEvent->posX), round(aCreateBuildingEvent->posY));
	std::tuple<int, int> buildingSize;

	glm::vec3 modelCenter = glm::vec3(-1.0f, -1.0f, -1.0f);

	bool isXRoundedUp = std::get<0>(closestToClick) >= aCreateBuildingEvent->posX;
	bool isYRoundedUp = std::get<1>(closestToClick) >= aCreateBuildingEvent->posY;

	switch (aCreateBuildingEvent->buildingType) {
		case DwellingID:
		{
			buildingSize = std::make_tuple(2, 2);
			break;
		}
		case LumberjackHutID:
		{
			buildingSize = std::make_tuple(2, 2);
			break;
		}
	}

	/* Calculate correct occupied units and save in fromX, toX, fromY, toY */
	/* Set correct 3d model center point */
	int fromX, toX, fromY, toY = 0;

	if (std::get<0>(buildingSize) % 2 == 0) {
		fromX = std::get<0>(closestToClick) - std::get<0>(buildingSize) / 2;
		toX = std::get<0>(closestToClick) + std::get<0>(buildingSize) / 2;

		modelCenter.x = std::get<0>(closestToClick);
	}
	else {
		if (isXRoundedUp) {
			fromX = std::get<0>(closestToClick) - std::get<0>(buildingSize) / 2 - 1;
			toX = std::get<0>(closestToClick) + std::get<0>(buildingSize) / 2 - 1;

			modelCenter.x = std::get<0>(closestToClick) - 0.5f;
		}
		else {
			fromX = std::get<0>(closestToClick) - std::get<0>(buildingSize) / 2;
			toX = std::get<0>(closestToClick) + std::get<0>(buildingSize) / 2;

			modelCenter.x = std::get<0>(closestToClick) + 0.5f;
		}
	}

	if (std::get<1>(buildingSize) % 2 == 0) {
		fromY = std::get<1>(closestToClick) - std::get<1>(buildingSize) / 2;
		toY = std::get<1>(closestToClick) + std::get<1>(buildingSize) / 2;

		modelCenter.y = std::get<1>(closestToClick);
	}
	else {
		if (isYRoundedUp) {
			fromY = std::get<1>(closestToClick) - std::get<1>(buildingSize) / 2 - 1;
			toY = std::get<1>(closestToClick) + std::get<1>(buildingSize) / 2 - 1;

			modelCenter.y = std::get<1>(closestToClick) - 0.5f;
		}
		else {
			fromY = std::get<1>(closestToClick) - std::get<1>(buildingSize) / 2;
			toY = std::get<1>(closestToClick) + std::get<1>(buildingSize) / 2;

			modelCenter.y = std::get<1>(closestToClick) + 0.5f;
		}
	}

	/* Check if the building is outside of the grid */
	if (fromX < 0 || toX >= grid->gridWidth || fromY < 0 || toY >= grid->gridHeight) {
		return;
	}

	/* Check if the grid is not occupied */
	for (int i = fromX; i < toX; ++i) {
		for (int j = fromY; j < toY; ++j) {
			if (grid->gridUnits[j][i]->occupied) {
				return;
			}
		}
	}

	/* Check if the floor is flat */
	if (!grid->IsAreaFlat(fromX, toX, fromY, toY)) {
		return;
	}

	/* set grid to occupied*/
	for (int i = fromX; i < toX; ++i) {
		for (int j = fromY; j < toY; ++j) {
			grid->gridUnits[j][i]->occupied = true;
		}
	}

	/* calculate 3d model position height*/
	modelCenter.z = grid->GetHeight(modelCenter.x, modelCenter.y);

	/* Create the building object etc.. */
	switch (aCreateBuildingEvent->buildingType) {
		case DwellingID:
		{
			/* create building  */
			Dwelling * dwelling = new Dwelling(modelCenter, // translate
				glm::vec3(0.012f, 0.006f, 0.012f), // rescale
				glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f)); // rotate

			/* save some stuff needed later.. TODO: dedicated building exit,check road etc (for other buildings)*/
			dwelling->fromX = fromX;
			dwelling->fromY = fromY;
			dwelling->toX = toX;
			dwelling->toY = toY;
			dwelling->sizeX = std::get<0>(buildingSize);
			dwelling->sizeY = std::get<1>(buildingSize);
			dwelling->entranceX = fromX + 1;
			dwelling->entranceY = fromY;

			dwelling->CreateBuildingOutline();
			/* create settler and link them.. TODO:*/
			Settler* settler = new Settler(glm::vec3(0 + 0.5f, 0 + 0.5f, grid->gridUnits[0][0]->averageHeight),
				glm::vec3(0.0045f, 0.0045f, 0.0045f), glm::vec3(0, 0, glm::pi<float>()));

			settler->SetDwelling(dwelling);


			Pathfinding path = Pathfinding(grid, Coordinate(0, 0), Coordinate(dwelling->entranceX, dwelling->entranceY));
			path.CalculatePath();
			std::list<Coordinate>pathShorts = path.GetPath();
			std::vector<glm::vec2> glmPath;
			glmPath.push_back(glm::vec2(0.5f, 0.5f));
			for (std::list<Coordinate>::iterator it = pathShorts.begin(); it != pathShorts.end(); ++it)
			{
				glmPath.push_back(glm::vec2((*it).first, (*it).second) + 0.5f);
				Lumberjack* pathLumby = new Lumberjack(glm::vec3((*it).first + 0.5f, (*it).second + 0.5f, grid->gridUnits[(*it).second][(*it).first]->averageHeight),
					glm::vec3(0.003f, 0.003f, 0.003f), glm::vec3(0, 0, glm::pi<float>()));
				grid->gridUnits[(*it).second][(*it).first]->movingObjects.push_back(pathLumby);
			}
			settler->SetNewPath(glmPath);

			/* save building in the coordinate where the 3d object center is located in->good for rendering */
			grid->gridUnits[(int)modelCenter.y][(int)modelCenter.x]->objects.push_back(dwelling);
			grid->gridUnits[0][0]->movingObjects.push_back(settler);
			break;
		}
		case LumberjackHutID:
		{
			/* save building in the coordinate where the 3d object center is located in -> good for rendering */
			LumberjackHut * lumberjackHut = new LumberjackHut(modelCenter,
				glm::vec3(0.14f, 0.14f, 0.14f), // rescale
				glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
			
			lumberjackHut->fromX = fromX;
			lumberjackHut->fromY = fromY;
			lumberjackHut->toX = toX;
			lumberjackHut->toY = toY;
			lumberjackHut->sizeX = std::get<0>(buildingSize);
			lumberjackHut->sizeY = std::get<1>(buildingSize);
			lumberjackHut->entranceX = fromX;
			lumberjackHut->entranceY = fromY;

			lumberjackHut->CreateBuildingOutline();

			/* create lumberjack */
			Lumberjack* lumby = new Lumberjack(glm::vec3(0 + 0.5f, 0 + 0.5f, grid->gridUnits[0][0]->averageHeight),
				glm::vec3(0.0045f, 0.0045f, 0.0045f), glm::vec3(0, 0, glm::pi<float>()));

			lumby->SetLumberjackHut(lumberjackHut);


			Pathfinding path = Pathfinding(grid, Coordinate(0, 0), Coordinate((int)modelCenter.x, (int)modelCenter.y));
			path.CalculatePath();
			std::list<Coordinate>pathShorts = path.GetPath();
			std::vector<glm::vec2> glmPath;
			glmPath.push_back(glm::vec2(0.5f, 0.5f));
			for (std::list<Coordinate>::iterator it = pathShorts.begin(); it != pathShorts.end(); ++it)
			{
				glmPath.push_back(glm::vec2((*it).first, (*it).second) + 0.5f);
			}
			lumby->SetNewPath(glmPath);

			// store reference to grid
			grid->gridUnits[(int)modelCenter.y][(int)modelCenter.x]->objects.push_back(lumberjackHut);
			grid->gridUnits[0][0]->movingObjects.push_back(lumby);
			break;
		}
	}
}
void EventHandler::Visit(DeleteEvent * aDeleteEvent)
{
	/* deletes reference to element */
	for (auto it = grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX]->movingObjects.begin(); it !=
		grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX]->movingObjects.end(); ++it) {
		if ((*it) == aDeleteEvent->gameObject) {
			it = grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX]->movingObjects.erase(it);
			break;
		}
	}
	for (auto it = grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX]->objects.begin(); it !=
		grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX]->objects.end(); ++it) {
		if ((*it) == aDeleteEvent->gameObject) {
			
			// to be moved into separate class 
			try {
				Tree* tree = dynamic_cast<Tree*>(*it);
				grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX]->hasTree = false;
			}
			catch (const std::exception& e) {}
			// ********************** //

			it = grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX]->objects.erase(it);
			break;
		}
	}
}

void EventHandler::Visit(GatherResourceEvent * aGatherResourceEvent)
{
	PathfindingObject path = PathfindingObject(grid, Coordinate(aGatherResourceEvent->person->position.x, aGatherResourceEvent->person->position.y));
	std::list<Coordinate> pathShorts;
	std::vector<glm::vec2> glmPath;

	switch (aGatherResourceEvent->resource)
	{
		case Wood:
			path.FindClosestTree();
			pathShorts = path.GetPath();
			glmPath.push_back(glm::vec2(aGatherResourceEvent->person->posX, aGatherResourceEvent->person->posY));
			for (std::list<Coordinate>::iterator it = pathShorts.begin(); it != pathShorts.end(); ++it)
			{
				glmPath.push_back(glm::vec2((*it).first, (*it).second) + 0.5f);
				//Lumberjack* pathLumby = new Lumberjack(glm::vec3((*it).first + 0.5f, (*it).second + 0.5f, grid->gridUnits[(*it).second][(*it).first]->averageHeight),
					//glm::vec3(0.003f, 0.003f, 0.003f), glm::vec3(0, 0, glm::pi<float>()));
				//grid->gridUnits[(*it).second][(*it).first]->movingObjects.push_back(pathLumby);
			}
			aGatherResourceEvent->person->SetNewPath(glmPath);
			aGatherResourceEvent->person->destination = path.GetDestinationObject();
			break;

		default:
			break;
	}
}