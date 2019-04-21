#include "stdafx.h"
#include "return_home_event.h"


ReturnHomeEvent::ReturnHomeEvent(BoneAnimated* aPerson, PersonType aPersonType)
{
	person = aPerson;
	personType = aPersonType;
}

void ReturnHomeEvent::Accept(GameEventVisitor* eventVisitor)
{
	eventVisitor->Visit(this);
}

