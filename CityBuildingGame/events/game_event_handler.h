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

class GameEventHandler : public GameEventVisitor
{
public:
	GameEventHandler::GameEventHandler(Grid* aGrid);
	void AddEvent(GameEvent*);
	GameEvent* GetEvent();
	bool ProcessEvent();

	void Visit(MoveEvent* aMoveEvent) override;
	void Visit(CreateBuildingEvent* aCreateBuildingEvent) override;
	bool ValidBuildingPosition(int fromX, int fromY, int toX, int toY) const; // Move to different class?
	void Visit(DeleteEvent* aDeleteEvent) override;
	void Visit(GatherResourceEvent* aGatherResourceEvent) override;
	void Visit(ReturnHomeEvent* aReturnHomeEvent) override;
	Grid* grid;

private:
	moodycamel::ConcurrentQueue<GameEvent*> cq;
};
