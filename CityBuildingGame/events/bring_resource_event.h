#pragma once
#include "game_event.h"

class Worker;

class BringResourceEvent : public GameEvent
{
public:
	BringResourceEvent(Worker* aWorker, int aPrevWorkerState);

	void Accept(GameEventVisitor* eventVisitor) override;

	Worker* worker;
	int prevWorkerState;
};
