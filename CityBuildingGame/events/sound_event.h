#pragma once
#include <sound_event_visitor.h>

class SoundEvent
{
public:
	virtual void Accept(SoundEventVisitor* sEventVisitor) = 0;
};
