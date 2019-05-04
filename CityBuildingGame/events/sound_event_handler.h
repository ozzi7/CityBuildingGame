#pragma once
#include <concurrentqueue.h>
#include <SFML/Audio.hpp>
#include <sound_event.h>
#include <play_sound_event.h>
#include <map>

class Grid;

class SoundEventHandler : public SoundEventVisitor {
public:
	SoundEventHandler::SoundEventHandler(int nofSounds);
	void SoundEventHandler::LoadFiles();
	void AddEvent(SoundEvent*);
	SoundEvent* GetEvent();
	bool ProcessEvent();
	void Visit(PlaySoundEvent* aPlaySoundEvent);

private:
	int maxSounds = 255;
	moodycamel::ConcurrentQueue<SoundEvent*> cq;
	sf::Music music;
	std::map<int, sf::SoundBuffer> soundBuffers;
	std::vector<sf::Sound> sounds;
};
