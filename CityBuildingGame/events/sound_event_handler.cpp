#include "stdafx.h"
#pragma once
#include <sound_event_handler.h>
#include <grid.h>

SoundEventHandler::SoundEventHandler(int nofSounds)
{
	// note 1 channel must be free for music as well.. total <= 256
	maxSounds = nofSounds;
	sounds = std::vector<sf::Sound>(nofSounds);
}

void SoundEventHandler::LoadFiles()
{
	if (music.openFromFile(Path + "/../music/TownTheme.ogg") && MUSIC_ENABLED)
	{
		// TODO: should be in another method to play..
		music.setLoop(true);
		music.play();
	}
	soundBuffers[0].loadFromFile(Path + "/../music/sound_effects/lvlup.ogg");
	soundBuffers[1].loadFromFile(Path + "/../music/sound_effects/tree_chopping.wav");
}
// TODO: make playMusicEvent to change music
void SoundEventHandler::AddEvent(SoundEvent* e)
{
	cq.enqueue(e);
}

SoundEvent* SoundEventHandler::GetEvent()
{
	SoundEvent* e;
	if (cq.try_dequeue(e))
	{
		return e;
	}
	return nullptr;
}

bool SoundEventHandler::ProcessEvent()
{
	SoundEvent* sound_event = GetEvent();
	if (sound_event != nullptr)
	{
		sound_event->Accept(this);
		return true;
	}
	return false;
}

void SoundEventHandler::Visit(PlaySoundEvent* aSoundEvent)
{
	if (!SOUND_ENABLED)
		return;

	int location = -1;
	for (int i = 0; i < sounds.size(); i++)
	{
		if (sounds[i].getStatus() != sf::Sound::Playing)
		{
			location = i;
			break;
		}
	}
	if (location != -1)
	{
		sounds[location].setBuffer(soundBuffers[int(aSoundEvent->soundType)]);
		sounds[location].play();
	}
}
