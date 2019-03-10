#include "stdafx.h"
#pragma once
#include <event_handler.h>

void EventHandler::AddEvent(Event * e)
{
	cq.enqueue(e);
}
Event * EventHandler::GetEvent()
{
	Event * e;
	if (cq.try_dequeue(e))
	{
		return e;
	}
	else
	{
		return NULL; // ??
	}
}