#pragma once
#include "game_event.h"

class BoneAnimated;
enum PersonType { LumberjackID };

class ReturnHomeEvent : public GameEvent
{
public:
	ReturnHomeEvent(BoneAnimated* aPerson, PersonType aPersonType);

	void Accept(GameEventVisitor* eventVisitor);

	BoneAnimated* person;
	PersonType personType;
};

