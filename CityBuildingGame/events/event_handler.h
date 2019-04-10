#pragma once
#include <concurrentqueue.h>
#include <event.h>
#include <move_event.h>
#include <create_building_event.h>
#include <delete_event.h>
#include <gather_resource_event.h>
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
	void Visit(DeleteEvent * aDeleteEvent);
	void Visit(GatherResourceEvent * aGatherResourceEvent);
	Grid * grid;

private:
	moodycamel::ConcurrentQueue<Event*> cq;
};
