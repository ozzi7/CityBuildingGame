#pragma once
#include <sound_event.h>

enum SoundType { SettlerArrivedID };

class PlaySoundEvent : public SoundEvent
{
public:
	PlaySoundEvent(SoundType);

	void PlaySoundEvent::Accept(SoundEventVisitor* sEventVisitor) override;

	SoundType soundType;
};
