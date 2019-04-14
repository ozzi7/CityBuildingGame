#include "stdafx.h"
#pragma once
#include <sound_event_handler.h>
#include <grid.h>

SoundEventHandler::SoundEventHandler(int nofSounds)
{
	// note 1 channel must be free for music as well.. total <= 256
	maxSounds = nofSounds;
}
void SoundEventHandler::LoadFiles()
{
	if (music.openFromFile(Path + "/../music/TownTheme.ogg"))
	{
		// TODO: should be in another method to play..
		music.setLoop(true);
		music.play();
	}
	soundBuffers[0].loadFromFile(Path + "/../music/sound_effects/lvlup.ogg");
}
// TODO: make playMusicEvent to change music
void SoundEventHandler::AddEvent(SoundEvent * e)
{
	cq.enqueue(e);
}
SoundEvent * SoundEventHandler::GetEvent()
{
	SoundEvent * e;
	if (cq.try_dequeue(e))
	{
		return e;
	}
	else
	{
		return NULL;
	}
}
bool SoundEventHandler::ProcessEvent()
{
	SoundEvent * sound_event = GetEvent();
	if (sound_event != NULL)
	{
		sound_event->Accept(this);
		return true;
	}
	return false;
}
void SoundEventHandler::Visit(PlaySoundEvent * aSoundEvent)
{
	if (sounds.size() == 0)
	{
		sounds.push_back(sf::Sound());
		sounds.at(0).setBuffer(soundBuffers[aSoundEvent->soundType]);
		sounds.at(0).play();
	}
	else
	{
		int location = -1;
		for (int i = 0; i < sounds.size(); i++)
		{
			if (!sounds.at(i).getStatus() != sf::Sound::Playing)
			{
				location = i;
				break;
			}
		}

		if (location != -1)
		{
			sounds.at(location).setBuffer(soundBuffers[aSoundEvent->soundType]);
			sounds.at(location).play();
		}
		else if(location == -1 && sounds.size() < maxSounds)
		{
			sounds.push_back(sf::Sound());
			sounds.at(sounds.size() - 1).setBuffer(soundBuffers[aSoundEvent->soundType]);
			sounds.at(sounds.size() - 1).play();
		}
	}
}
