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
												  grid->GetHeight(pathCoordinates.front().first, pathCoordinates.front().second)),
			                            glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(0, 0, glm::pi<float>()));

			worker->SetDwelling(dwelling);
			worker->SetNewPath(pathCoordinates);
			worker->state = State::immigrating;

			/* save building in the coordinate where the 3d object center is located in->good for rendering */
			grid->gridUnits[dwelling->entranceY][dwelling->entranceX].objects.push_back(dwelling);
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
			grid->gridUnits[lumberjackHut->entranceY][lumberjackHut->entranceX].objects.push_back(lumberjackHut);
			resources->AddLumberjackHut(lumberjackHut);
			resources->AddWorkerTask(lumberjackHut);

			break;
		}
	}
}

void GameEventHandler::AssignWorkToIdleWorkers()
{
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
			PathfindingObject* pathFindingObj = new PathfindingObject(grid, std::pair<int, int>(worker->posX, worker->posY));
			pathFindingObj->FindResourceUsage(); // TODO: find closest resource

			if (pathFindingObj.resourceBuilding != nullptr && pathFindingObj.targetBuilding != nullptr) {
				Pathfinding* pathFinding = new Pathfinding(grid, std::pair<int, int>(worker->posX, worker->posY),
					std::pair<int, int>(pathFindingObj.resourceBuilding->posX, pathFindingObj.resourceBuilding->posY)); // path from worker to resource

				std::list<std::pair<int, int>> pathCoordinatesList = pathFinding->GetPath();
				std::vector<std::pair<int, int>> pathCoordinates{ std::make_move_iterator(std::begin(pathCoordinatesList)),
					std::make_move_iterator(std::end(pathCoordinatesList)) };

				delete pathFinding;

				worker->SetNewPath(pathCoordinates);
				worker->state = State::gettingResource;
				worker->destination = pathFindingObj.resourceBuilding;
				worker->resourceTargetBuilding = pathFindingObj.targetBuilding;

				grid->gridUnits[worker->posY][worker->posX].movingObjects.push_back(worker);
				worker->destination = pathFindingObj.targetBuilding->woodOnTheWay++;
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
					std::vector<std::pair<int, int>> pathCoordinates{ std::make_move_iterator(std::begin(pathCoordinatesList)),
																	std::make_move_iterator(std::end(pathCoordinatesList)) };

					try {
						lumberjackHut = dynamic_cast<LumberjackHut*>(pathFinding->GetDestinationObject());
					}
					catch (const std::exception & e) {} // Not an exception, expected behavior...

					if (lumberjackHut)
					{
						worker->destination = lumberjackHut;
						lumberjackHut->workersOnTheWay++;

						worker->SetNewPath(pathCoordinates);
						worker->state = State::goingToWork;
						worker->destination = lumberjackHut;

						if (lumberjackHut->workersPresent + lumberjackHut->workersOnTheWay == lumberjackHut->workersRequired)
							resources->RemoveWorkerTask(lumberjackHut);
					}
				}
				// add worker back into idleWorkers, if it could not be assigned a task
				else
				{
					resources->AddIdleWorker(worker);
					return;
				}
				delete pathFinding;
			}
			delete pathFindingObj;
			/* ... until here*/
		}
	}
	else if (!resources->idleWorkers.empty() && !resources->workerTasks.empty() &&
			 resources->idleWorkers.size() > resources->workerTasks.size())
	{
		while (Building* building = resources->GetWorkerTask())
		{
			LumberjackHut* lumberjackHut = nullptr;
			Worker* worker = nullptr;
			PathfindingObject* pathFinding = new PathfindingObject(grid, std::pair<int,int>(building->posX, building->posY));
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

				delete pathFinding;
				
				try {
					lumberjackHut = dynamic_cast<LumberjackHut*>(building);
				}
				catch(const std::exception& e) {} // Not an exception, expected behavior...

				if (lumberjackHut)
				{
					// copy worker position to new lumby
					Lumberjack* lumby = new Lumberjack(glm::vec3(worker->posX, worker->posY, grid->GetHeight(worker->posX, worker->posY)),
													   glm::vec3(0.6f, 0.6f, 0.6f),
													   glm::vec3(0, 0, glm::pi<float>()));

					lumby->SetLumberjackHut(lumberjackHut);
					lumberjackHut->workersPresent++;

					lumby->SetNewPath(pathCoordinates);
					lumby->state = State::walking;
					lumby->SetLumberjackHut(lumberjackHut);
					lumby->destination = lumberjackHut;

					// store reference to lumby
					grid->gridUnits[lumby->posY][lumby->posX].movingObjects.push_back(lumby);

					resources->AddLumberjack(lumby);

					resources->RemoveIdleWorker(worker);

					// add building back into workerTasks, number of required workers is not yet reached
					if (lumberjackHut->workersPresent < lumberjackHut->workersRequired)
						resources->AddWorkerTask(building);
				}
				// add building back into workerTasks, if it could not be assigned a task
				else 
				{
					resources->AddWorkerTask(building);
					return;
				}
			}
			// add building back into workerTasks, if it could not be assigned a task
			else 
			{
				resources->AddWorkerTask(building);
				return;
			}
		}
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
	lumberjackHut->Evolve;

	if (lumberjackHut->workersRequired == lumberjackHut->workersPresent + lumberjackHut->workersOnTheWay) {
		// all workers arrived

		Building* building = nullptr;
		try {
			building = dynamic_cast<Building*>(aWorkerArrivedEvent->gameObject);
		}
		catch (const std::exception & e)  // This should not happen
		{
			return;
		}
		resources->RemoveWorkerTask(building); // building has enough workers now (TODO: )
	}

	// copy worker position to new lumby
	Lumberjack* lumby = new Lumberjack(glm::vec3(lumberjackHut->entranceX, lumberjackHut->entranceY, 
		grid->GetHeight(lumberjackHut->entranceX, lumberjackHut->entranceY)),
		glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3(0, 0, glm::pi<float>()));

	lumby->SetLumberjackHut(lumberjackHut);
	lumby->state = State::idle;
	lumby->destination = lumberjackHut;
	lumby->visible = false;

	if (lumberjackHut->workersRequired == lumberjackHut->workersPresent + lumberjackHut->workersOnTheWay) { // last lumby starts working
		lumby->visible = true;
		unitEventHandler->AddEvent(new GatherResourceEvent(Resource::Wood, lumby));
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
		case Wood:
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
			grid, std::pair<int,int>(aReturnHomeEvent->person->position.x, aReturnHomeEvent->person->position.y),
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
