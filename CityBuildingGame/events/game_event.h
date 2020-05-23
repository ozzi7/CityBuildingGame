#pragma once
#include <game_event_visitor.h>

enum class Resource { Wood, Stone };
enum class BuildingType { LumberjackHutID, DwellingID };

class GameEvent
{
public:
	virtual void Accept(GameEventVisitor* eventVisitor) = 0;
};
