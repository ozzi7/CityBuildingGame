#include "stdafx.h"
#pragma once
#include <game_event_handler.h>
#include <grid.h>
#include <bone_animated.h>
#include <dwelling.h>
#include <lumberjack.h>
#include <settler.h>
#include <lumberjack_hut.h>
#include <pathfinding.h>
#include <pathfinding_object.h>

GameEventHandler::GameEventHandler(Grid* aGrid)
{
	grid = aGrid;
}

void GameEventHandler::AddEvent(GameEvent* e)
{
	cq.enqueue(e);
}

GameEvent* GameEventHandler::GetEvent()
{
	GameEvent* e;
	if (cq.try_dequeue(e))
	{
		return e;
	}
	return nullptr;
}

bool GameEventHandler::ProcessEvent()
{
	GameEvent* game_event = GetEvent();
	if (game_event != nullptr)
	{
		game_event->Accept(this);
		return true;
	}
	return false;
}

void GameEventHandler::Visit(MoveEvent* aMoveEvent)
{
	/* removes element found by reference */
	BoneAnimated* toMove = nullptr;
	for (auto it = grid->gridUnits[aMoveEvent->fromY][aMoveEvent->fromX].movingObjects.begin(); it !=
	     grid->gridUnits[aMoveEvent->fromY][aMoveEvent->fromX].movingObjects.end(); ++it)
	{
		if (*it == aMoveEvent->gameObject)
		{
			toMove = *it;
			it = grid->gridUnits[aMoveEvent->fromY][aMoveEvent->fromX].movingObjects.erase(it);
			grid->gridUnits[aMoveEvent->toY][aMoveEvent->toX].movingObjects.push_back(toMove);
			break;
		}
	}
}

void GameEventHandler::Visit(CreateBuildingEvent* aCreateBuildingEvent)
{
	std::pair<int, int> closestToClick = std::make_pair(round(aCreateBuildingEvent->posX),
	                                                      round(aCreateBuildingEvent->posY));
	std::pair<int, int> buildingSize;

	glm::vec3 modelCenter = glm::vec3(-1.0f, -1.0f, -1.0f);

	bool isXRoundedUp = std::get<0>(closestToClick) >= aCreateBuildingEvent->posX;
	bool isYRoundedUp = std::get<1>(closestToClick) >= aCreateBuildingEvent->posY;

	switch (aCreateBuildingEvent->buildingType)
	{
		case BuildingType::DwellingID:
		{
			buildingSize = std::make_pair(2, 2);
			break;
		}
		case BuildingType::LumberjackHutID:
		{
			buildingSize = std::make_pair(2, 2);
			break;
		}
	}

	/* Calculate correct occupied units and save in fromX, toX, fromY, toY */
	/* Set correct 3d model center point */
	int fromX, toX, fromY, toY = 0;

	if (std::get<0>(buildingSize) % 2 == 0)
	{
		fromX = std::get<0>(closestToClick) - std::get<0>(buildingSize) / 2;
		toX = std::get<0>(closestToClick) + std::get<0>(buildingSize) / 2;

		modelCenter.x = std::get<0>(closestToClick);
	}
	else
	{
		if (isXRoundedUp)
		{
			fromX = std::get<0>(closestToClick) - std::get<0>(buildingSize) / 2 - 1;
			toX = std::get<0>(closestToClick) + std::get<0>(buildingSize) / 2 - 1;

			modelCenter.x = std::get<0>(closestToClick) - 0.5f;
		}
		else
		{
			fromX = std::get<0>(closestToClick) - std::get<0>(buildingSize) / 2;
			toX = std::get<0>(closestToClick) + std::get<0>(buildingSize) / 2;

			modelCenter.x = std::get<0>(closestToClick) + 0.5f;
		}
	}

	if (std::get<1>(buildingSize) % 2 == 0)
	{
		fromY = std::get<1>(closestToClick) - std::get<1>(buildingSize) / 2;
		toY = std::get<1>(closestToClick) + std::get<1>(buildingSize) / 2;

		modelCenter.y = std::get<1>(closestToClick);
	}
	else
	{
		if (isYRoundedUp)
		{
			fromY = std::get<1>(closestToClick) - std::get<1>(buildingSize) / 2 - 1;
			toY = std::get<1>(closestToClick) + std::get<1>(buildingSize) / 2 - 1;

			modelCenter.y = std::get<1>(closestToClick) - 0.5f;
		}
		else
		{
			fromY = std::get<1>(closestToClick) - std::get<1>(buildingSize) / 2;
			toY = std::get<1>(closestToClick) + std::get<1>(buildingSize) / 2;

			modelCenter.y = std::get<1>(closestToClick) + 0.5f;
		}
	}

	if (!grid->ValidBuildingPosition(fromX, fromY, toX, toY))
		return;

	/* calculate 3d model position height*/
	modelCenter.z = grid->GetHeight(modelCenter.x, modelCenter.y);

	/* set grid to occupied*/
	for (int i = fromX; i < toX; ++i)
	{
		for (int j = fromY; j < toY; ++j)
		{
			grid->gridUnits[j][i].occupied = true;
		}
	}

	/* Create the building object etc.. */
	switch (aCreateBuildingEvent->buildingType)
	{
		case BuildingType::DwellingID:
		{
			/* find path*/
			PathfindingObject* path = new PathfindingObject(grid, std::pair<int,int>(fromX + 1, fromY));
			path->FindClosestEdge();
			std::list<std::pair<int,int>> pathCoordinatesList = path->GetPath();
			pathCoordinatesList.reverse();
			std::vector<std::pair<int,int>> pathCoordinates {std::make_move_iterator(std::begin(pathCoordinatesList)), 
															 std::make_move_iterator(std::end(pathCoordinatesList))};
			delete path;

			/* if no path found do nothing..*/
			if (pathCoordinates.empty())
				return;
			
			/* create building  */
			Dwelling* dwelling = new Dwelling(modelCenter, // translate
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

			/* create settler.. */
			Settler* settler = new Settler(glm::vec3(pathCoordinates.front().first + 0.5f, pathCoordinates.front().second + 0.5f,
													 grid->GetHeight(pathCoordinates.front().first, pathCoordinates.front().second)),
			                               glm::vec3(0.45f, 0.45f, 0.45f), glm::vec3(0, 0, glm::pi<float>()));

			settler->SetDwelling(dwelling);
			settler->SetNewPath(pathCoordinates);

			/* save building in the coordinate where the 3d object center is located in->good for rendering */
			grid->gridUnits[(int)modelCenter.y][(int)modelCenter.x].objects.push_back(dwelling);
			grid->gridUnits[pathCoordinates.back().second][pathCoordinates.back().first].movingObjects.push_back(settler);
			resources->AddDwelling(dwelling);
			resources->AddIdleSettler(settler);
			resources->AddSettler(settler);
			break;
		}
		case BuildingType::LumberjackHutID:
		{
			LumberjackHut* lumberjackHut = new LumberjackHut(modelCenter, // translate
			                                                 glm::vec3(0.012f, 0.006f, 0.012f), // rescale
			                                                 glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f)); // rotate

			lumberjackHut->fromX = fromX;
			lumberjackHut->fromY = fromY;
			lumberjackHut->toX = toX;
			lumberjackHut->toY = toY;
			lumberjackHut->sizeX = std::get<0>(buildingSize);
			lumberjackHut->sizeY = std::get<1>(buildingSize);
			lumberjackHut->entranceX = fromX + 1;
			lumberjackHut->entranceY = fromY;

			lumberjackHut->CreateBuildingOutline();

			/*get settlers if there are enough, here we get 2 settlers, kill them and create 1 lumby */
			std::vector<Settler*> settlers = resources->GetIdleSettlers(2);
			if (settlers.size() != 0)
			{
				// copy first settlers position to new lumby
				Lumberjack* lumby = new Lumberjack(glm::vec3(settlers[0]->posX, settlers[0]->posY,
				                                             grid->GetHeight(settlers[0]->posX, settlers[0]->posY)),
				                                   glm::vec3(0.45f, 0.45f, 0.45f), glm::vec3(0, 0, glm::pi<float>()));

				lumby->SetLumberjackHut(lumberjackHut);

				/* there should always be a path here because of roads */
				Pathfinding* path = new Pathfinding(grid, std::pair<int,int>(settlers[0]->posX, settlers[0]->posY),
				                                      std::pair<int,int>(lumberjackHut->entranceX, lumberjackHut->entranceY));
				path->CalculatePath();
				std::list<std::pair<int,int>> pathCoordinatesList = path->GetPath();
				std::vector<std::pair<int,int>> pathCoordinates {std::make_move_iterator(std::begin(pathCoordinatesList)), 
																		std::make_move_iterator(std::end(pathCoordinatesList))};
				delete path;

				if (pathCoordinates.size() != 0)
				{
					lumby->SetNewPath(pathCoordinates);
					lumby->state = State::returningHome;
					lumby->SetLumberjackHut(lumberjackHut);
					lumby->destination = lumberjackHut;
				}
				else
				{
					loggingEventHandler->AddEvent(
						new LoggingEvent(LoggingLevel::ERROR_L, "The lumberjack can't walk from dwelling to lumberjackhut (no path)"));
				}

				// store reference to lumby
				grid->gridUnits[lumby->posY][lumby->posX].movingObjects.push_back(lumby);

				/* delete the settlers */
				for (int i = 0; i < settlers.size(); ++i)
				{
					this->AddEvent(new DeleteEvent(settlers[i]->posX, settlers[i]->posY, settlers[i])); // kill the settlers
				}
				resources->AddLumberjack(lumby);
			}

			// store reference to grid
			grid->gridUnits[(int)modelCenter.y][(int)modelCenter.x].objects.push_back(lumberjackHut);
			resources->AddLumberjackHut(lumberjackHut);

			break;
		}
	}
}

void GameEventHandler::Visit(DeleteEvent* aDeleteEvent)
{
	/* deletes reference to element */
	for (auto it = grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX].movingObjects.begin(); it !=
	     grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX].movingObjects.end(); ++it)
	{
		if (*it == aDeleteEvent->gameObject)
		{
			it = grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX].movingObjects.erase(it);
			break;
		}
	}
	for (auto it = grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX].objects.begin(); it !=
	     grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX].objects.end(); ++it)
	{
		if (*it == aDeleteEvent->gameObject)
		{
			it = grid->gridUnits[aDeleteEvent->posY][aDeleteEvent->posX].objects.erase(it);
			break;
		}
	}
	// TODO: delete the object in resources
	resources->RemoveObject(aDeleteEvent->gameObject);
}

void GameEventHandler::Visit(GatherResourceEvent* aGatherResourceEvent)
{
	PathfindingObject* path = new PathfindingObject(
		grid, std::pair<int,int>(aGatherResourceEvent->person->position.x, aGatherResourceEvent->person->position.y));
	
	switch (aGatherResourceEvent->resource)
	{
		case Wood:
		{
			path->FindClosestTree();
			std::list<std::pair<int,int>> pathCoordinatesList = path->GetPath();
			std::vector<std::pair<int,int>> pathCoordinates{std::make_move_iterator(std::begin(pathCoordinatesList)), 
															std::make_move_iterator(std::end(pathCoordinatesList))};
			
			if (pathCoordinates.size() != 0)
			{
				grid->gridUnits[path->GetDestinationObject()->posY][path->GetDestinationObject()->posX].hasTree = false;
				aGatherResourceEvent->person->SetNewPath(pathCoordinates);
				aGatherResourceEvent->person->destination = path->GetDestinationObject();
				aGatherResourceEvent->person->state = State::walkingToTarget;
			}
			else
			{
				aGatherResourceEvent->person->state = State::idle;
			}
			break;
		}
		default:
			break;
	}
	delete path;
}

void GameEventHandler::Visit(ReturnHomeEvent* aReturnHomeEvent)
{
	switch (aReturnHomeEvent->personType)
	{
	case PersonType::LumberjackID:
		Lumberjack* lumby = (Lumberjack*)aReturnHomeEvent->person; // or extract coordinates??
		Pathfinding* path = new Pathfinding(
			grid, std::pair<int,int>(aReturnHomeEvent->person->position.x, aReturnHomeEvent->person->position.y),
			std::pair<int,int>(lumby->lumberjackHut->entranceX, lumby->lumberjackHut->entranceY));
		path->CalculatePath();
		std::list<std::pair<int,int>> pathCoordinatesList = path->GetPath();
		std::vector<std::pair<int,int>> pathCoordinates{std::make_move_iterator(std::begin(pathCoordinatesList)), 
														std::make_move_iterator(std::end(pathCoordinatesList))};
		delete path;

		lumby->SetNewPath(pathCoordinates);
		lumby->state = State::returningHome;
		break;
	}
}
