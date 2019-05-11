#pragma once

class PlaySoundEvent;

class SoundEventVisitor
{
public:
	virtual void Visit(PlaySoundEvent* aPlaySoundEvent) = 0;
};
