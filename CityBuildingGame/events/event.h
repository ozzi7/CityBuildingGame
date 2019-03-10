#pragma once

enum eventType { MoveObjectEvent, DeleteObjectEvent };

class Event
{
public:
	Event();
	Event(eventType MoveObject, int aFromX, int aFromY, int aToX, int aToY, int aIndex);
	
	int fromX;
	int fromY;
	int toX; 
	int toY;
	int index;
};