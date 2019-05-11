#include "stdafx.h"
#pragma once
#include "play_sound_event.h"

PlaySoundEvent::PlaySoundEvent(SoundType aSoundType)
{
	soundType = aSoundType;
};

void PlaySoundEvent::Accept(SoundEventVisitor* soundEventVisitor)
{
	soundEventVisitor->Visit(this);
};
