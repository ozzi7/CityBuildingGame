#pragma once
#include <game_event_visitor.h>

class GameEvent
{
public:
	virtual void Accept(GameEventVisitor* eventVisitor) = 0;
};
