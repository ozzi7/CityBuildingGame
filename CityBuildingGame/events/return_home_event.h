#pragma once
#include "game_event.h"

class BoneAnimated;

enum class PersonType { LumberjackID };

class ReturnHomeEvent : public GameEvent
{
public:
	ReturnHomeEvent(BoneAnimated* aPerson, PersonType aPersonType);

	void Accept(GameEventVisitor* eventVisitor) override;

	BoneAnimated* person;
	PersonType personType;
};
