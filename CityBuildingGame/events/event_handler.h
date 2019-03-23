#pragma once
#include <concurrentqueue.h>
#include <event.h>
#include <move_event.h>
#include <create_building_event.h>
#include <tuple>


class Grid;

class EventHandler : public EventVisitor {
public:
	EventHandler::EventHandler(Grid * aGrid);
	void AddEvent(Event*);
	Event * GetEvent();
	bool ProcessEvent();

	void Visit(MoveEvent * aMoveEvent);
	void Visit(CreateBuildingEvent * aCreateBuildingEvent);
	Grid * grid;

private:
	moodycamel::ConcurrentQueue<Event*> cq;
};
