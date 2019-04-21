#pragma once
#include <concurrentqueue.h>
#include <game_event.h>
#include <move_event.h>
#include <create_building_event.h>
#include <delete_event.h>
#include <gather_resource_event.h>
#include <return_home_event.h>
#include <tuple>


class Grid;

class GameEventHandler : public GameEventVisitor {
public:
	GameEventHandler::GameEventHandler(Grid * aGrid);
	void AddEvent(GameEvent*);
	GameEvent * GetEvent();
	bool ProcessEvent();

	void Visit(MoveEvent * aMoveEvent);
	void Visit(CreateBuildingEvent * aCreateBuildingEvent);
	void Visit(DeleteEvent * aDeleteEvent);
	void Visit(GatherResourceEvent * aGatherResourceEvent);
	void Visit(ReturnHomeEvent * aReturnHomeEvent);
	Grid * grid;

private:
	moodycamel::ConcurrentQueue<GameEvent*> cq;
};
