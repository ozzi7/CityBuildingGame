#pragma once

enum eventType { MoveObjectEvent, DeleteObjectEvent, CreateBuilding};

class Event
{
public:
	Event();
	Event(eventType MoveObject, int aFromX, int aFromY, int aToX, int aToY, int aIndex);
	Event(eventType CreateBuilding, int posX, int posY);
	int fromX;
	int fromY;
	int toX; 
	int toY;
	int index;
	int posX;
	int posY;
	eventType eventID;
};