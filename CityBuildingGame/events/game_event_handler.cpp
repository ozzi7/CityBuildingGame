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
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "[EVENT] Move (" + std::to_string(aMoveEvent->fromX) +
	", " + std::to_string(aMoveEvent->fromY) + ") => (" + std::to_string(aMoveEvent->toX) + ", " + std::to_string(aMoveEvent->toY)+")"));

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
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::DEBUG, "[EVENT] Create building"));

	std::pair<int, int> closestToClick = grid->GetClosestValidPosition(aCreateBuildingEvent->StartPosition);
	std::pair<int, int> closestToClickEnd = grid->GetClosestValidPosition(aCreateBuildingEvent->EndPosition);

	std::pair<int, int> buildingSize;

	glm::vec3 modelCenter = glm::vec3(-1.0f, -1.0f, -1.0f);
	switch (aCreateBuildingEvent->buildingType)
	{
	case BuildingType::DwellingID:
	case BuildingType::LumberjackHutID:
		buildingSize = std::make_pair(3, 3);
		break;
	case BuildingType::PathID:
		buildingSize = std::make_pair(1, 1);
		break;
	}

	/* Calculate correct occupied units and save in fromX, toX, fromY, toY inclusive */
	/* Set correct 3d model center point */
	int fromX, toX, fromY, toY = 0;
	int targetFromX, targetToX, targetFromY, targetToY = 0;
	if (buildingSize.first % 2 == 0)
	{
		fromX = closestToClick.first - buildingSize.first / 2;
		toX = closestToClick.first + buildingSize.first / 2 - 1;
		modelCenter.x = closestToClick.first;

		targetFromX = closestToClickEnd.first - buildingSize.first / 2;
		targetToX = closestToClickEnd.first + buildingSize.first / 2 - 1;
	}
	else
	{
		fromX = int(aCreateBuildingEvent->StartPosition.first) - buildingSize.first / 2;
		toX = int(aCreateBuildingEvent->StartPosition.first) + buildingSize.first / 2;
		modelCenter.x = int(aCreateBuildingEvent->StartPosition.first) + 0.5f;

		targetFromX = int(aCreateBuildingEvent->EndPosition.first) - buildingSize.first / 2;
		targetToX = int(aCreateBuildingEvent->EndPosition.first) + buildingSize.first / 2;
	}

	if (buildingSize.second % 2 == 0)
	{
		fromY = closestToClick.second - buildingSize.second / 2;
		toY = closestToClick.second + buildingSize.second / 2 - 1;
		modelCenter.y = closestToClick.second;

		targetFromY = closestToClickEnd.second - buildingSize.second / 2;
		targetToY = closestToClickEnd.second + buildingSize.second / 2 - 1;
	}
	else
	{
		fromY = int(aCreateBuildingEvent->StartPosition.second) - buildingSize.second / 2;
		toY = int(aCreateBuildingEvent->StartPosition.second) + buildingSize.second / 2;
		modelCenter.y = int(aCreateBuildingEvent->StartPosition.second) + 0.5f;

		targetFromY = int(aCreateBuildingEvent->EndPosition.second) - buildingSize.second / 2;
		targetToY = int(aCreateBuildingEvent->EndPosition.second) + buildingSize.second / 2;
	}

	/* calculate 3d model position height*/
	modelCenter.z = grid->GetHeight(modelCenter.x, modelCenter.y);

	/* Create the building object etc.. */
	switch (aCreateBuildingEvent->buildingType)
	{
		case BuildingType::DwellingID:
		{
			if (!grid->IsValidBuildingPosition(fromX, fromY, toX, toY))
				return;

			std::pair<int, int> entrance = grid->FindRoadAccess(fromX, toX, fromY, toY);

			if (entrance.first == -1)
			{
				loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::WARNING, "Cannot find road to building"));
				return;
			}

			/* find path*/
			PathfindingObject* path = new PathfindingObject(grid, std::pair<int,int>(entrance.first, entrance.second));
			path->FindClosestEdge();
			std::list<std::pair<int,int>> pathCoordinatesList = path->GetPath();
			pathCoordinatesList.reverse();
			std::vector<std::pair<int,int>> pathCoordinates {std::make_move_iterator(std::begin(pathCoordinatesList)), 
															 std::make_move_iterator(std::end(pathCoordinatesList))};
			delete path;

			/* if no path found do nothing..*/
			if (pathCoordinates.empty())
			{
				loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::WARNING, "The worker can't walk to the dwelling (no path found)"));
				return;
			}


			grid->SetIsOccupied(fromX, toX, fromY, toY, true);

			/* create building  */
			Dwelling* dwelling = new Dwelling(modelCenter, // translate
			                                  glm::vec3(0.014f, 0.008f, 0.014f), // rescale
			                                  glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f),  // rotate
												modelCenter.z);

			/* save some stuff needed later.. TODO: dedicated building exit,check road etc (for other buildings)*/
			dwelling->fromX = fromX;
			dwelling->fromY = fromY;
			dwelling->toX = toX;
			dwelling->toY = toY;
			dwelling->sizeX = std::get<0>(buildingSize);
			dwelling->sizeY = std::get<1>(buildingSize);
			dwelling->entranceX = entrance.first;
			dwelling->entranceY = entrance.second;
			dwelling->AddWoodBuildingMaterialOnTheWay(2);

			dwelling->CreateBuildingOutline();
			grid->DeleteGrass(fromX, fromY, toX, toY);

			/* create worker.. */
			Worker* worker = new Worker(glm::vec3(pathCoordinates.front().first + 0.5f, pathCoordinates.front().second + 0.5f, 
												  grid->GetHeight(pathCoordinates.front().first + 0.5f, pathCoordinates.front().second + 0.5f)),
			                            glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(0, 0, glm::pi<float>()));

			worker->dwelling = dwelling;
			worker->SetNewPath(pathCoordinates);
			worker->state = State::immigrating;

			/* save building in the coordinate where the 3d object center is located in->good for rendering */
			grid->gridUnits[dwelling->posY][dwelling->posX].objects.push_back(dwelling);
			grid->gridUnits[pathCoordinates.front().second][pathCoordinates.front().first].movingObjects.push_back(worker);
			resources->AddDwelling(dwelling);
			resources->AddWorker(worker);
			soundEventHandler->AddEvent(new PlaySoundEvent(SoundType::WorkerArrivedSound));

			break;
		}
		case BuildingType::LumberjackHutID:
		{
			if (!grid->IsValidBuildingPosition(fromX, fromY, toX, toY))
				return;

			std::pair<int, int> entrance = grid->FindRoadAccess(fromX, toX, fromY, toY);

			if (entrance.first == -1)
			{
				loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::WARNING, "Cannot find road to building"));
				return;
			}

			modelCenter.x = modelCenter.x - 0.45f;
			LumberjackHut* lumberjackHut = new LumberjackHut(modelCenter, // translate
			                                                 glm::vec3(0.012f, 0.006f, 0.012f), // rescale
			                                                 glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f),  // rotate
															modelCenter.z);

			lumberjackHut->fromX = fromX;
			lumberjackHut->fromY = fromY;
			lumberjackHut->toX = toX;
			lumberjackHut->toY = toY;
			lumberjackHut->sizeX = std::get<0>(buildingSize);
			lumberjackHut->sizeY = std::get<1>(buildingSize);
			lumberjackHut->entranceX = entrance.first;
			lumberjackHut->entranceY = entrance.second;

			lumberjackHut->CreateBuildingOutline();

			grid->SetIsOccupied(fromX, toX, fromY, toY, true);
			grid->DeleteGrass(fromX, fromY, toX, toY);

			// store reference to grid
			grid->gridUnits[lumberjackHut->posY][lumberjackHut->posX].objects.push_back(lumberjackHut);
			resources->AddLumberjackHut(lumberjackHut);
			resources->AddWorkerTask(lumberjackHut);
			soundEventHandler->AddEvent(new PlaySoundEvent(SoundType::WorkerArrivedSound));

			break;
		}
		case BuildingType::PathID:
		{
			grid->ClearRoadPreview();

			// only allow building roads connected to other roads or on the border of the map
			if ((targetFromX != fromX || targetFromY != fromY))
			{
				// we have a path from a to b here
				if (!grid->IsOccupied(targetFromX, targetFromY) && !grid->IsOccupied(fromX, fromY))
				{
					// the endpoints are not occupied
					Pathfinding* pathFinding = new Pathfinding(grid, std::pair<int, int>(fromX, fromY),
						std::pair<int, int>(targetFromX, targetFromY));
					pathFinding->CalculatePath();
					std::list<std::pair<int, int>> pathCoordinatesList = pathFinding->GetPath();
					std::vector<std::pair<int, int>> pathCoordinates{ std::make_move_iterator(std::begin(pathCoordinatesList)),
																		std::make_move_iterator(std::end(pathCoordinatesList)) };
					delete pathFinding;

					if (!pathCoordinates.empty() && (grid->HasRoadAccess(pathCoordinates) || grid->IsAtEdgeOfMap(pathCoordinates)))
					{ // there is a path with access from outside the map
						grid->SetHasRoad(pathCoordinates, true);
						grid->DeleteGrass(pathCoordinates);
					}
				}
				break;
			}
			else if (!grid->IsOccupied(targetFromX, targetFromY))
			{ // in this case we have clicked on a single tile
				if (grid->HasRoadAccess(fromX, fromY) || grid->IsAtEdgeOfMap(fromX, fromY))
				{
					grid->SetHasRoad(fromX, fromY, true);
					grid->DeleteGrass(fromX, fromY, fromX, fromY);
				}
			}
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
			pathFindingRes->FindResourceFromWorker();

			if (pathFindingRes->resourceBuilding != nullptr && pathFindingRes->targetBuilding != nullptr) 
			{
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

				pathFindingRes->resourceBuilding->ReserveWoodBuildingMaterial();
				worker->resourceTargetBuilding->AddWoodBuildingMaterialOnTheWay();
				if (worker->goingHome)
				{
					if (!worker->dwelling->RemoveWorkerOnTheWay());
						loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::ERROR_L, "Expected workerOnTheWay to be > 0"));
				}
				else
				{
					if (!worker->dwelling->RemoveWorker());
						loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::ERROR_L, "Expected workerPresent to be > 0"));
				}

				// Remove from tasks when everything is on the way
				if (pathFindingRes->targetBuilding->AllRequiredWorkersOnTheWay() && pathFindingRes->targetBuilding->AllRequiredBuildingMaterialsOnTheWay())
					resources->RemoveWorkerTask(pathFindingRes->targetBuilding);
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
						lumberjackHut->AddWorkerOnTheWay();

						worker->SetNewPath(pathCoordinates);
						worker->state = State::goingToWork;
						worker->visible = true;
						worker->destination = lumberjackHut;
						if (worker->goingHome)
						{
							if (!worker->dwelling->RemoveWorkerOnTheWay());
								loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::ERROR_L, "Expected workerOnTheWay to be > 0"));
						}
						else
						{
							if (!worker->dwelling->RemoveWorker());
								loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::ERROR_L, "Expected workerPresent to be > 0"));
						}

						// Remove from tasks when everything is on the way
						if (lumberjackHut->AllRequiredWorkersOnTheWay() && lumberjackHut->AllRequiredBuildingMaterialsOnTheWay())
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
			if (!building->AllRequiredBuildingMaterialsOnTheWay()) 
			{
				PathfindingResource* pathFindingRes = new PathfindingResource(grid, std::pair<int, int>(building->entranceX, building->entranceY));
				pathFindingRes->targetBuilding = building;
				pathFindingRes->FindResourceFromBuilding();
				
				if (pathFindingRes->resourceBuilding != nullptr && pathFindingRes->targetWorker != nullptr) 
				{
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

					pathFindingRes->resourceBuilding->ReserveWoodBuildingMaterial();
					building->AddWoodBuildingMaterialOnTheWay();

					resources->RemoveIdleWorker(pathFindingRes->targetWorker);
					if (pathFindingRes->targetWorker->goingHome)
					{
						if (!pathFindingRes->targetWorker->dwelling->RemoveWorkerOnTheWay());
							loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::ERROR_L, "Expected workerOnTheWay to be > 0"));
					}
					else
					{
						if (!pathFindingRes->targetWorker->dwelling->RemoveWorker());
							loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::ERROR_L, "Expected workerPresent to be > 0"));
					}


					// add building back into workerTasks if number of required workers or resources is not yet reached
					if (!building->AllRequiredWorkersOnTheWay() ||
						!building->AllRequiredBuildingMaterialsOnTheWay())
					{
						resources->AddWorkerTask(building);
					}
					delete pathFindingRes;
					continue;
				}
				delete pathFindingRes;
			}
			/* ... until here*/

			/* all other cases (going to work etc)*/
			if (!building->AllRequiredWorkersOnTheWay())
			{
				LumberjackHut* lumberjackHut = nullptr;
				Worker* worker = nullptr;
				PathfindingObject* pathFinding = new PathfindingObject(grid, std::pair<int,int>(building->entranceX, building->entranceY));
				pathFinding->FindClosestIdleWorker();
				std::list<std::pair<int,int>> pathCoordinatesList = pathFinding->GetPath();
				
				if (!pathCoordinatesList.empty())
				{
					pathCoordinatesList.reverse();
					std::vector<std::pair<int,int>> pathCoordinates{std::make_move_iterator(std::begin(pathCoordinatesList)), 
																    std::make_move_iterator(std::end(pathCoordinatesList))};

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
						lumberjackHut->AddWorkerOnTheWay();

						worker->SetNewPath(pathCoordinates);
						worker->state = State::goingToWork;
						worker->visible = true;
						worker->destination = lumberjackHut;

						resources->RemoveIdleWorker(worker);
						if (worker->goingHome)
						{
							if (!worker->dwelling->RemoveWorkerOnTheWay());
								loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::ERROR_L, "Expected workerOnTheWay to be > 0"));
						}
						else
						{
							if (!worker->dwelling->RemoveWorker());
								loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::ERROR_L, "Expected workerPresent to be > 0"));
						}

					}
					// add building back into workerTasks if number of required workers or resources is not yet reached
					if (!building->AllRequiredWorkersOnTheWay() ||
						!building->AllRequiredBuildingMaterialsOnTheWay())
					{
						resources->AddWorkerTask(building);
					}
				}
				// add building back into workerTasks, if it could not be assigned a task
				// to prevent infinite loop, add it to temporary list first
				else 
				{
					tempWorkerTasks.push_back(building);
				}
				delete pathFinding;
			}
		}
		for (Building* it : tempWorkerTasks)
			resources->AddWorkerTask(it);
	}
}

void GameEventHandler::SetWorkerStateIdle(Worker* worker)
{
	worker->state = State::idle;
	resources->AddIdleWorker(worker);
	
	PathfindingObject* pathFinding = new PathfindingObject(grid, std::make_pair(worker->posX, worker->posY));
	pathFinding->FindClosestIdleDwelling();
	std::list<std::pair<int,int>> pathCoordinatesList = pathFinding->GetPath();
	
	if (!pathCoordinatesList.empty())
	{
		try {
			Dwelling* dwelling = dynamic_cast<Dwelling*>(pathFinding->GetDestinationObject());
		
			std::vector<std::pair<int,int>> pathCoordinates{std::make_move_iterator(std::begin(pathCoordinatesList)), 
														    std::make_move_iterator(std::end(pathCoordinatesList))};
			worker->SetNewPath(pathCoordinates);
			worker->dwelling = dwelling;
			worker->goingHome = true;
			dwelling->AddWorkerOnTheWay();
		}
		catch(const std::exception& e)  // This should not happen
		{
			loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::ERROR_L, "Expected dwelling from pathfinding is not a dwelling"));
		}
	}
	delete pathFinding;
}

void GameEventHandler::Visit(DeleteEvent* aDeleteEvent)
{
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, "[EVENT] Delete"));

	Tree* tree = nullptr;
	try {
		tree = dynamic_cast<Tree*>(aDeleteEvent->gameObject);
		if(tree)
			grid->SetIsOccupied(aDeleteEvent->posX, aDeleteEvent->posX, aDeleteEvent->posY, aDeleteEvent->posY, false);
	}
	catch (const std::exception & e)  // This should happen
	{
	}

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
	lumberjackHut->AddWorker();

	// copy worker position to new lumby
	Lumberjack* lumby = new Lumberjack(glm::vec3(lumberjackHut->entranceX + 0.5f, lumberjackHut->entranceY + 0.5f,
		grid->GetHeight(lumberjackHut->entranceX + 0.5f, lumberjackHut->entranceY + 0.5f)),
		glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3(0, 0, glm::pi<float>()));

	lumby->SetLumberjackHut(lumberjackHut);
	lumby->state = State::idle;
	lumby->destination = lumberjackHut; // is this needed?
	lumby->visible = false;

	// if the last required worker arrives then let him become visible and start gathering wood
	if (lumberjackHut->AllRequiredWorkersOnSite() &&
		lumberjackHut->AllRequiredBuildingMaterialsOnSite())
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
				grid->SetHasTree(path->GetDestinationObject()->posX, path->GetDestinationObject()->posY, false);
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

				if (building->RemoveWoodBuildingMaterial())
				{
					worker->visible = true;
					worker->state = State::carryingWood;

					try {
						Dwelling* dwelling = dynamic_cast<Dwelling*>(worker->destination);
						if (dwelling)
							dwelling->UpdateWoodModels(); // to update the 3d model shown next to dwelling.. TODO
					}
					catch (const std::exception & e) {} // Not an exception. Expected behavior...

				}
				else
				{
					SetWorkerStateIdle(worker);
					return;
				}
				break;

			case (1):

				if (building->RemoveStoneBuildingMaterial())
				{
					worker->visible = true;
					worker->state = State::carryingStone;
				}
				else
				{
					SetWorkerStateIdle(worker);
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

	if (lumberjackHut)
	{
		if (lumberjackHut->AllRequiredBuildingMaterialsOnSite() &&
			lumberjackHut->AllRequiredWorkersOnSite())
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
}
void GameEventHandler::Visit(CreateBuildingPreviewEvent* aCreateBuildingPreviewEvent)
{
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::NOTSET, "[EVENT] Create building preview"));

	grid->previewObjects.clear();
	grid->ClearRoadPreview();

	std::pair<int, int> closestToClick = grid->GetClosestValidPosition(aCreateBuildingPreviewEvent->StartPosition);
	std::pair<int, int> closestToClickEnd = grid->GetClosestValidPosition(aCreateBuildingPreviewEvent->EndPosition);

	std::pair<int, int> buildingSize;

	glm::vec3 modelCenter = glm::vec3(-1.0f, -1.0f, -1.0f);
	switch (aCreateBuildingPreviewEvent->buildingType)
	{
		case BuildingType::DwellingID:			
		case BuildingType::LumberjackHutID:
			buildingSize = std::make_pair(3, 3);
			break;
		case BuildingType::PathID:
			buildingSize = std::make_pair(1, 1);
			break;	
	}
	
	/* Calculate correct occupied units and save in fromX, toX, fromY, toY inclusive */
	/* Set correct 3d model center point */
	int fromX, toX, fromY, toY = 0;
	int targetFromX, targetToX, targetFromY, targetToY = 0;
	if (buildingSize.first % 2 == 0)
	{
		fromX = closestToClick.first - buildingSize.first / 2;
		toX = closestToClick.first + buildingSize.first / 2 - 1;
		modelCenter.x = closestToClick.first;

		targetFromX = closestToClickEnd.first - buildingSize.first / 2;
		targetToX = closestToClickEnd.first + buildingSize.first / 2 - 1;
	}
	else
	{
		fromX = int(aCreateBuildingPreviewEvent->StartPosition.first) - buildingSize.first / 2;
		toX = int(aCreateBuildingPreviewEvent->StartPosition.first) + buildingSize.first / 2;
		modelCenter.x = int(aCreateBuildingPreviewEvent->StartPosition.first) + 0.5f;

		targetFromX = int(aCreateBuildingPreviewEvent->EndPosition.first) - buildingSize.first / 2;
		targetToX = int(aCreateBuildingPreviewEvent->EndPosition.first) + buildingSize.first / 2;
	}

	if (buildingSize.second % 2 == 0)
	{
		fromY = closestToClick.second - buildingSize.second / 2;
		toY = closestToClick.second + buildingSize.second / 2 - 1;
		modelCenter.y = closestToClick.second;

		targetFromY = closestToClickEnd.second - buildingSize.second / 2;
		targetToY = closestToClickEnd.second + buildingSize.second / 2 - 1;
	}
	else
	{
		fromY = int(aCreateBuildingPreviewEvent->StartPosition.second) - buildingSize.second / 2;
		toY = int(aCreateBuildingPreviewEvent->StartPosition.second) + buildingSize.second / 2;
		modelCenter.y = int(aCreateBuildingPreviewEvent->StartPosition.second) + 0.5f;

		targetFromY = int(aCreateBuildingPreviewEvent->EndPosition.second) - buildingSize.second / 2;
		targetToY = int(aCreateBuildingPreviewEvent->EndPosition.second) + buildingSize.second / 2;
	}

	/* calculate 3d model position height*/
	modelCenter.z = grid->GetHeight(modelCenter.x, modelCenter.y);

	/* Create the building object etc.. */
	switch (aCreateBuildingPreviewEvent->buildingType)
	{
		case BuildingType::DwellingID:
		{
			if (!grid->IsValidBuildingPosition(fromX, fromY, toX, toY))
				return;
			

			std::pair<int, int> entrance = grid->FindRoadAccess(fromX, toX, fromY, toY);

			if (entrance.first == -1)
			{
				return;
			}

			/* create building  */
			Dwelling* dwelling = new Dwelling(modelCenter, // translate
				glm::vec3(0.014f, 0.008f, 0.014f), // rescale
				glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f),  // rotate
				modelCenter.z);
			dwelling->fromX = fromX;
			dwelling->fromY = fromY;
			dwelling->toX = toX;
			dwelling->toY = toY;
			dwelling->sizeX = std::get<0>(buildingSize);
			dwelling->sizeY = std::get<1>(buildingSize);
			dwelling->Evolve();

			grid->previewObjects.push_back(dwelling);

			break;
		}
		case BuildingType::LumberjackHutID:
		{
			if (!grid->IsValidBuildingPosition(fromX, fromY, toX, toY))
				return;

			std::pair<int, int> entrance = grid->FindRoadAccess(fromX, toX, fromY, toY);

			if (entrance.first == -1)
			{
				return;
			}

			modelCenter.x = modelCenter.x - 0.45f;
			LumberjackHut* lumberjackHut = new LumberjackHut(modelCenter, // translate
				glm::vec3(0.012f, 0.006f, 0.012f), // rescale
				glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f),  // rotate
				modelCenter.z);

			lumberjackHut->fromX = fromX;
			lumberjackHut->fromY = fromY;
			lumberjackHut->toX = toX;
			lumberjackHut->toY = toY;
			lumberjackHut->sizeX = std::get<0>(buildingSize);
			lumberjackHut->sizeY = std::get<1>(buildingSize);

			lumberjackHut->Evolve();

			grid->previewObjects.push_back(lumberjackHut);

			break;
		}
		case BuildingType::PathID:
		{
			// check if the path has different start & endpoint and then check if endpoint is not occupied (pathfinding allows it)
			if (targetFromX != fromX || targetFromY != fromY) {
				// we have a path from a to b here
				if (!grid->IsOccupied(targetFromX, targetFromY) && !grid->IsOccupied(fromX, fromY))
				{
					Pathfinding* pathFinding = new Pathfinding(grid, std::pair<int, int>(fromX, fromY),
						std::pair<int, int>(targetFromX, targetFromY));
					pathFinding->CalculatePath();
					std::list<std::pair<int, int>> pathCoordinatesList = pathFinding->GetPath();
					std::vector<std::pair<int, int>> pathCoordinates{ std::make_move_iterator(std::begin(pathCoordinatesList)),
																	  std::make_move_iterator(std::end(pathCoordinatesList)) };

					delete pathFinding;

					if (!pathCoordinates.empty() && (grid->HasRoadAccess(pathCoordinates) || grid->IsAtEdgeOfMap(pathCoordinates)))
					{ // there is a path with access from outside the map
						grid->SetHasRoadPreview(pathCoordinates, true);
					}
				}
			}
			else if (!grid->IsOccupied(fromX, fromY))
				// in this case we have clicked on a single tile
				if (grid->HasRoadAccess(fromX, fromY) || grid->IsAtEdgeOfMap(fromX, fromY))
				{
					grid->SetHasRoadPreview(fromX, fromY, true);
				}
			break;
		}
	}
}