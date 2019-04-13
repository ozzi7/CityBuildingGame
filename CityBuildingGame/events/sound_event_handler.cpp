#include "stdafx.h"
#pragma once
#include <sound_event_handler.h>
#include <grid.h>

SoundEventHandler::SoundEventHandler()
{
	if (!music.openFromFile(Path + "/../music/TownTheme.ogg"))
	{
	}
	music.setLoop(true);
	music.play();
}
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
	//Event * game_event = GetEvent();
	//if (game_event != NULL)
	//{
	//	game_event->Accept(this);
	//	return true;
	//}
	return false;
}
void SoundEventHandler::Visit(PlaySoundEvent * aSoundEvent)
{
	bool bla = true;
}
