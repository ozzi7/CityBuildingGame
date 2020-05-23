#include "stdafx.h"
#pragma once
#include <game_event_handler.h>
#include <grid.h>
#include <bone_animated.h>
#include <dwelling.h>
#include <lumberjack.h>
#include <worker.h>
#include <lumberjack_hut.h>
#include <pathfinding.h>
#include <pathfinding_object.h>
#include "pathfinding_resource.h"

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
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "[EVENT] Move"));

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
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, "[EVENT] Create building"));

	std::pair<int, int> closestToClick = std::make_pair(round(aCreateBuildingEvent->posX),
	                                                      round(aCreateBuildingEvent->posY));
	std::pair<int, int> buildingSize;

	glm::vec3 modelCenter = glm::vec3(-1.0f, -1.0f, -1.0f);
	switch (aCreateBuildingEvent->buildingType)
	{
		case BuildingType::DwellingID:
		{
			buildingSize = std::make_pair(3, 3);
			break;
		}
		case BuildingType::LumberjackHutID:
		{
			buildingSize = std::make_pair(3, 3);
			break;
		}
	}

	/* Calculate correct occupied units and save in fromX, toX, fromY, toY inclusive */
	/* Set correct 3d model center point */
	int fromX, toX, fromY, toY = 0;

	if (std::get<0>(buildingSize) % 2 == 0)
	{
		fromX = std::get<0>(closestToClick) - std::get<0>(buildingSize) / 2;
		toX = std::get<0>(closestToClick) + std::get<0>(buildingSize)/ 2 - 1;
		modelCenter.x = std::get<0>(closestToClick);
	}
	else
	{
		fromX = int(aCreateBuildingEvent->posX) - std::get<0>(buildingSize) / 2;
		toX = int(aCreateBuildingEvent->posX) + std::get<0>(buildingSize) / 2;
		modelCenter.x = int(aCreateBuildingEvent->posX) + 0.5f;
	}

	if (std::get<1>(buildingSize) % 2 == 0)
	{
		fromY = std::get<1>(closestToClick) - std::get<1>(buildingSize) / 2;
		toY = std::get<1>(closestToClick) + std::get<1>(buildingSize) / 2 - 1;
		modelCenter.y = std::get<1>(closestToClick);
	}
	else
	{
		fromY = int(aCreateBuildingEvent->posY) - std::get<1>(buildingSize) / 2;
		toY = int(aCreateBuildingEvent->posY) + std::get<1>(buildingSize) / 2;
		modelCenter.y = int(aCreateBuildingEvent->posY) + 0.5f;
	}

	if (!grid->IsValidBuildingPosition(fromX, fromY, toX, toY))
		return;

	/* calculate 3d model position height*/
	modelCenter.z = grid->GetHeight(modelCenter.x, modelCenter.y);

	grid->SetGridOccupied(fromX, toX, fromY, toY);

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
			{
				grid->SetGridFree(fromX, toX, fromY, toY);
				loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::WARNING, "The worker can't walk to the dwelling (no path found)"));
				return;
			}
			
			/* create building  */
			Dwelling* dwelling = new Dwelling(modelCenter, // translate
			                                  glm::vec3(0.014f, 0.008f, 0.014f), // rescale
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

			/* create worker.. */
			Worker* worker = new Worker(glm::vec3(pathCoordinates.front().first + 0.5f, pathCoordinates.front().second + 0.5f, 
												  grid->GetHeight(pathCoordinates.front().first + 0.5f, pathCoordinates.front().second + 0.5f)),
			                            glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(0, 0, glm::pi<float>()));

			worker->SetDwelling(dwelling);
			worker->SetNewPath(pathCoordinates);
			worker->state = State::immigrating;

			/* save building in the coordinate where the 3d object center is located in->good for rendering */
			grid->gridUnits[dwelling->posY][dwelling->posX].objects.push_back(dwelling);
			grid->gridUnits[pathCoordinates.back().second][pathCoordinates.back().first].movingObjects.push_back(worker);
			resources->AddDwelling(dwelling);
			resources->AddWorker(worker);
			break;
		}
		case BuildingType::LumberjackHutID:
		{
			modelCenter.x = modelCenter.x - 0.45f;
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

			// store reference to grid
			grid->gridUnits[lumberjackHut->posY][lumberjackHut->posX].objects.push_back(lumberjackHut);
			resources->AddLumberjackHut(lumberjackHut);
			resources->AddWorkerTask(lumberjackHut);

			break;
		}
	}
}

void GameEventHandler::AssignWorkToIdleWorkers()
{
	std::list<Building*> tempWorkerTasks;
	/* 
	 * If there are fewer idle workers (probably 1) than idle buildings:
	 * find nearest idle building for (each) idle worker.
	 * If there are fewer idle buildings (probably 1) than workers:
	 * find nearest idle worker for (each) idle building.
	 */
	if (!resources->idleWorkers.empty() && !resources->workerTasks.empty() &&
		resources->idleWorkers.size() <= resources->workerTasks.size())
	{
		while (Worker* worker = resources->GetIdleWorker())
		{
			/* bring resource to a building task... */
			PathfindingResource* pathFindingRes = new PathfindingResource(grid, std::pair<int, int>(worker->posX, worker->posY));
			pathFindingRes->FindResourceFromWorker(); // TODO: find closest resource

			if (pathFindingRes->resourceBuilding != nullptr && pathFindingRes->targetBuilding != nullptr) {
				Pathfinding* pathFinding = new Pathfinding(grid, std::pair<int, int>(worker->posX, worker->posY),
														   std::pair<int, int>(pathFindingRes->resourceBuilding->entranceX, 
														                       pathFindingRes->resourceBuilding->entranceY)); // path from worker to resource
				pathFinding->CalculatePath();
				std::list<std::pair<int, int>> pathCoordinatesList = pathFinding->GetPath();
				std::vector<std::pair<int, int>> pathCoordinates {std::make_move_iterator(std::begin(pathCoordinatesList)),
																  std::make_move_iterator(std::end(pathCoordinatesList))};

				delete pathFinding;

				worker->SetNewPath(pathCoordinates);
				worker->state = State::gettingWood; // TODO wood or stone..?
				worker->visible = true;

				worker->destination = pathFindingRes->resourceBuilding;
				worker->resourceTargetBuilding = pathFindingRes->targetBuilding;

				//grid->gridUnits[worker->posY][worker->posX].movingObjects.push_back(worker);
				pathFindingRes->targetBuilding->woodOnTheWay++;
			}
			/* ... until here*/

			/* all other cases (going to work etc)*/
			else
			{
				LumberjackHut* lumberjackHut = nullptr;
				PathfindingObject* pathFinding = new PathfindingObject(grid, std::pair<int, int>(worker->posX, worker->posY));
				pathFinding->FindClosestIdleBuilding();
				std::list<std::pair<int, int>> pathCoordinatesList = pathFinding->GetPath();

				if (!pathCoordinatesList.empty())
				{
					std::vector<std::pair<int, int>> pathCoordinates{std::make_move_iterator(std::begin(pathCoordinatesList)),
																	 std::make_move_iterator(std::end(pathCoordinatesList))};

					try {
						lumberjackHut = dynamic_cast<LumberjackHut*>(pathFinding->GetDestinationObject());
					}
					catch (const std::exception & e) {} // Not an exception. Expected behavior...

					if (lumberjackHut)
					{
						worker->destination = lumberjackHut;
						lumberjackHut->workersOnTheWay++;

						worker->SetNewPath(pathCoordinates);
						worker->state = State::goingToWork;
						worker->visible = true;
						worker->destination = lumberjackHut;

						if (lumberjackHut->workersPresent + lumberjackHut->workersOnTheWay == lumberjackHut->workersRequired)
							resources->RemoveWorkerTask(lumberjackHut);
					}
				}
				// add worker back into idleWorkers, if it could not be assigned a task
				else
				{
					resources->AddIdleWorker(worker);
					delete pathFinding;
					delete pathFindingRes;
					return;
				}
				delete pathFinding;
			}
			delete pathFindingRes;
			/* ... until here*/
		}
	}
	
	else if (!resources->idleWorkers.empty() && !resources->workerTasks.empty() &&
			 resources->idleWorkers.size() > resources->workerTasks.size())
	{
		while (Building* building = resources->GetWorkerTask())
		{
			/* bring resource to a building task... */
			PathfindingResource* pathFindingRes = new PathfindingResource(grid, std::pair<int, int>(building->entranceX, building->entranceY));
			pathFindingRes->FindResourceFromBuilding(); // TODO: find closest resource

			if (pathFindingRes->resourceBuilding != nullptr && pathFindingRes->targetWorker != nullptr) {
				Pathfinding* pathFinding = new Pathfinding(grid, 
														   std::pair<int, int>(pathFindingRes->targetWorker->posX, pathFindingRes->targetWorker->posY),
														   std::pair<int, int>(pathFindingRes->resourceBuilding->entranceX,
														                       pathFindingRes->resourceBuilding->entranceY)); // path from worker to resource
				pathFinding->CalculatePath();
				std::list<std::pair<int, int>> pathCoordinatesList = pathFinding->GetPath();
				std::vector<std::pair<int, int>> pathCoordinates{std::make_move_iterator(std::begin(pathCoordinatesList)),
																 std::make_move_iterator(std::end(pathCoordinatesList))};

				delete pathFinding;

				pathFindingRes->targetWorker->SetNewPath(pathCoordinates);
				pathFindingRes->targetWorker->state = State::gettingWood; // TODO wood or stone..?
				pathFindingRes->targetWorker->visible = true;
				pathFindingRes->targetWorker->destination = pathFindingRes->resourceBuilding;
				pathFindingRes->targetWorker->resourceTargetBuilding = building;

				//grid->gridUnits[pathFindingRes->targetWorker->posY][pathFindingRes->targetWorker->posX].movingObjects.push_back(pathFindingRes->targetWorker);
				building->woodOnTheWay++;

				// add building back into workerTasks if number of required workers or resources is not yet reached
				if (building->workersPresent < building->workersRequired ||
					building->woodStored + building->woodOnTheWay < building->woodRequired ||
					building->stoneStored + building->stoneOnTheWay < building->stoneRequired)
				{
					resources->AddWorkerTask(building);
				}
			}
			/* ... until here*/

			/* all other cases (going to work etc)*/
			else
			{
				LumberjackHut* lumberjackHut = nullptr;
				Worker* worker = nullptr;
				PathfindingObject* pathFinding = new PathfindingObject(grid, std::pair<int,int>(building->entranceX, building->entranceY));
				pathFinding->FindClosestIdleWorker();
				std::list<std::pair<int,int>> pathCoordinatesList = pathFinding->GetPath();
				
				if (!pathCoordinatesList.empty())
				{
					std::vector<std::pair<int,int>> pathCoordinates{std::make_move_iterator(std::end(pathCoordinatesList)), 
																    std::make_move_iterator(std::begin(pathCoordinatesList))};

					try {
						worker = dynamic_cast<Worker*>(pathFinding->GetDestinationObject());
					}
					catch(const std::exception& e)  // This should not happen
					{
						loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::ERROR_L, "Expected worker from pathfinding is not a worker"));
					}
					
					try {
						lumberjackHut = dynamic_cast<LumberjackHut*>(building);
					}
					catch(const std::exception& e) {} // Not an exception. Expected behavior...

					if (lumberjackHut)
					{
						worker->destination = lumberjackHut;
						lumberjackHut->workersOnTheWay++;

						worker->SetNewPath(pathCoordinates);
						worker->state = State::goingToWork;
						worker->visible = true;
						worker->destination = lumberjackHut;

						resources->RemoveIdleWorker(worker);

					}
					// add building back into workerTasks if number of required workers or resources is not yet reached
					if (building->workersPresent < building->workersRequired ||
						building->woodStored + building->woodOnTheWay < building->woodRequired ||
						building->stoneStored + building->stoneOnTheWay < building->stoneRequired)
					{
						tempWorkerTasks.push_back(building);
					}
				}
				// add building back into workerTasks, if it could not be assigned a task
				else 
				{
					resources->AddWorkerTask(building);
					delete pathFinding;
					delete pathFindingRes;
					return;
				}
				delete pathFinding;
			}
			delete pathFindingRes;
		}
		for (Building* it : tempWorkerTasks)
			resources->AddWorkerTask(it);
	}
}

void GameEventHandler::Visit(DeleteEvent* aDeleteEvent)
{
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, "[EVENT] Delete"));

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
	resources->RemoveObject(aDeleteEvent->gameObject);
	delete aDeleteEvent->gameObject;
}

void GameEventHandler::Visit(WorkerArrivedEvent* aWorkerArrivedEvent)
{
	LumberjackHut* lumberjackHut = nullptr;
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, "[EVENT] Worker arrived at workplace"));
	try {
		lumberjackHut = dynamic_cast<LumberjackHut*>(aWorkerArrivedEvent->gameObject);
	}
	catch (const std::exception & e)  // This should not happen
	{
		return;
	}
	lumberjackHut->workersPresent++;
	lumberjackHut->workersOnTheWay--;

	// copy worker position to new lumby
	Lumberjack* lumby = new Lumberjack(glm::vec3(lumberjackHut->entranceX+0.5f, lumberjackHut->entranceY + 0.5f,
		grid->GetHeight(lumberjackHut->entranceX + 0.5f, lumberjackHut->entranceY + 0.5f)),
		glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3(0, 0, glm::pi<float>()));

	lumby->SetLumberjackHut(lumberjackHut);
	lumby->state = State::idle;
	lumby->destination = lumberjackHut; // is this needed?
	lumby->visible = false;

	// if the last required worker arrives then let him become visible and start gathering wood
	if (lumberjackHut->woodStored >= lumberjackHut->woodRequired &&
		lumberjackHut->stoneStored >= lumberjackHut->stoneRequired &&
		lumberjackHut->workersPresent == lumberjackHut->workersRequired)
	{
		gameEventHandler->AddEvent(new GatherResourceEvent(Resource::Wood, lumby));
		lumby->visible = true;
	}

	// store reference to lumby
	grid->gridUnits[lumby->posY][lumby->posX].movingObjects.push_back(lumby);
	resources->AddLumberjack(lumby);
}

void GameEventHandler::Visit(GatherResourceEvent* aGatherResourceEvent)
{
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, "[EVENT] Gather resource"));

	PathfindingObject* path = new PathfindingObject(
		grid, std::pair<int,int>(aGatherResourceEvent->person->position.x, aGatherResourceEvent->person->position.y));
	
	switch (aGatherResourceEvent->resource)
	{
		case Resource::Wood:
		{
			path->FindClosestTree();
			std::list<std::pair<int,int>> pathCoordinatesList = path->GetPath();
			
			if (pathCoordinatesList.size() != 0)
			{
				std::vector<std::pair<int,int>> pathCoordinates{std::make_move_iterator(std::begin(pathCoordinatesList)), 
															    std::make_move_iterator(std::end(pathCoordinatesList))};
				grid->gridUnits[path->GetDestinationObject()->posY][path->GetDestinationObject()->posX].hasTree = false;
				aGatherResourceEvent->person->SetNewPath(pathCoordinates);
				aGatherResourceEvent->person->destination = path->GetDestinationObject();
				aGatherResourceEvent->person->state = State::goingToWork;
				aGatherResourceEvent->person->visible = true;
			}
			else
				aGatherResourceEvent->person->state = State::idle;
			break;
		}
		default:
			break;
	}
	delete path;
}

void GameEventHandler::Visit(ReturnHomeEvent* aReturnHomeEvent)
{
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, "[EVENT] Return home"));

	switch (aReturnHomeEvent->personType)
	{
	case PersonType::LumberjackID:
		Lumberjack* lumby = (Lumberjack*)aReturnHomeEvent->person; // or extract coordinates??
		Pathfinding* path = new Pathfinding(
			grid, std::pair<int,int>(lumby->position.x, lumby->position.y),
			std::pair<int,int>(lumby->lumberjackHut->entranceX, lumby->lumberjackHut->entranceY));
		path->CalculatePath();
		std::list<std::pair<int,int>> pathCoordinatesList = path->GetPath();
		std::vector<std::pair<int,int>> pathCoordinates{std::make_move_iterator(std::begin(pathCoordinatesList)), 
														std::make_move_iterator(std::end(pathCoordinatesList))};
		delete path;

		lumby->SetNewPath(pathCoordinates);
		lumby->state = State::carryingWood;
		break;
	}
}
void GameEventHandler::Visit(BringResourceEvent* aBringResourceEvent)
{
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, "[EVENT] Bring resource"));


	Worker* worker = aBringResourceEvent->worker;
	Pathfinding* path = new Pathfinding(grid, std::pair<int, int>(worker->position.x, worker->position.y),
										std::pair<int, int>(worker->resourceTargetBuilding->entranceX, worker->resourceTargetBuilding->entranceY));
	path->CalculatePath();
	std::list<std::pair<int, int>> pathCoordinatesList = path->GetPath();
	delete path;

	if (!pathCoordinatesList.empty())
	{
		std::vector<std::pair<int, int>> pathCoordinates{std::make_move_iterator(std::begin(pathCoordinatesList)),
														 std::make_move_iterator(std::end(pathCoordinatesList))};

		Building* building;
		try {
			building = dynamic_cast<Building*>(worker->destination);
		}
		catch (const std::exception & e)  // This should not happen
		{
			return;
		}

		switch (aBringResourceEvent->prevWorkerState)
		{
			case (0) :
				worker->state = State::carryingWood;

				if (building->woodStored - building->woodRequired > 0)
				{
					worker->visible = true;
					building->woodStored--;
				}
				else
				{
					worker->state = State::idle;
					resources->AddIdleWorker(worker);
					return;
				}
				break;

			case (1):
				worker->state = State::carryingStone;
				worker->visible = true;

				if (building->stoneStored - building->stoneRequired > 0)
				{
					building->stoneStored--;
				}
				else
				{
					worker->visible = true;
					worker->state = State::idle;
					resources->AddIdleWorker(worker);
					return;
				}
				break;
		}
		worker->SetNewPath(pathCoordinates);
		worker->destination = worker->resourceTargetBuilding;
	}
}
void GameEventHandler::Visit(ResourceArrivedEvent* aResourceArrivedEvent)
{
	// but resources are counted up in gameobject not here, only the lumberjack is generated here...TODO !?

	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, "[EVENT] Resource arrived"));

	LumberjackHut* lumberjackHut = nullptr;
	try {
		lumberjackHut = dynamic_cast<LumberjackHut*>(aResourceArrivedEvent->targetGameObject);
	}
	catch (const std::exception & e)  // This should not happen
	{
		return;
	}

	if (lumberjackHut->woodStored >= lumberjackHut->woodRequired &&
		lumberjackHut->stoneStored >= lumberjackHut->stoneRequired &&
		lumberjackHut->workersPresent == lumberjackHut->workersRequired)
	{
		// copy entrance pos
		Lumberjack* lumby = new Lumberjack(glm::vec3(lumberjackHut->entranceX + 0.5f, lumberjackHut->entranceY + 0.5f,
			grid->GetHeight(lumberjackHut->entranceX + 0.5f, lumberjackHut->entranceY + 0.5f)),
			glm::vec3(0.6f, 0.6f, 0.6f),
			glm::vec3(0, 0, glm::pi<float>()));

		lumby->SetLumberjackHut(lumberjackHut);
		lumby->state = State::idle;
		lumby->destination = lumberjackHut; // is this needed?
		lumby->visible = true;

		// store reference to lumby
		grid->gridUnits[lumby->posY][lumby->posX].movingObjects.push_back(lumby);
		resources->AddLumberjack(lumby);
		gameEventHandler->AddEvent(new GatherResourceEvent(Resource::Wood, lumby));
	}
}