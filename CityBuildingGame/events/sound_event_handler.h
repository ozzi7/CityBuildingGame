#pragma once
#include <concurrentqueue.h>
#include <SFML/Audio.hpp>
#include <sound_event.h>
#include <play_sound_event.h>

class Grid;

class SoundEventHandler : public SoundEventVisitor {
public:
	SoundEventHandler::SoundEventHandler();
	void SoundEventHandler::LoadFiles();
	void AddEvent(SoundEvent*);
	SoundEvent * GetEvent();
	bool ProcessEvent();
	void Visit(PlaySoundEvent * aPlaySoundEvent);


private:
	moodycamel::ConcurrentQueue<SoundEvent*> cq;
	sf::Music music;
};
