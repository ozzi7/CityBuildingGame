#pragma once
#include <sound_event.h>

enum class SoundType { WorkerArrivedSound, TreeChoppingSound };

class PlaySoundEvent : public SoundEvent
{
public:
	PlaySoundEvent(SoundType);

	void PlaySoundEvent::Accept(SoundEventVisitor* sEventVisitor) override;

	SoundType soundType;
};
