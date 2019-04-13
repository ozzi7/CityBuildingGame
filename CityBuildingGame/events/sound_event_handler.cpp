#include "stdafx.h"
#pragma once
#include <sound_event_handler.h>
#include <grid.h>

SoundEventHandler::SoundEventHandler()
{

}
void SoundEventHandler::LoadFiles()
{
	if (music.openFromFile(Path + "/../music/TownTheme.ogg"))
	{
		// TODO: should be in another method to play..
		music.setLoop(true);
		//music.play();
	}
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
	bool bla = true;
}
