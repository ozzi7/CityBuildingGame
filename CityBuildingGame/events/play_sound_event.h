#pragma once
#include <sound_event.h>

//enum BuildingType { LumberjackHutID, DwellingID };

class PlaySoundEvent : public SoundEvent
{
public:
	PlaySoundEvent();

	void PlaySoundEvent::Accept(SoundEventVisitor* sEventVisitor);
};