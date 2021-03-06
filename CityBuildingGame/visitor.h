#pragma once

class Tree;
class RenderedObject;
class Oak;
class Grass;
class Toona;
class EuroBeech;
class EuroBeech2;
class Pine;
class Terrain;
class Lumberjack;
class Building;
class LumberjackHut;
class Dwelling;
class Worker;

class Visitor
{
public:
	virtual void Visit(Terrain* terrain) = 0;

	virtual void Visit(Tree* tree) = 0;
	virtual void Visit(RenderedObject* renderedObject) = 0;
	virtual void Visit(Oak* oak) = 0;
	virtual void Visit(EuroBeech* euroBeech) = 0;
	virtual void Visit(EuroBeech2* euroBeech) = 0;
	virtual void Visit(Pine* pine) = 0;
	virtual void Visit(Toona* toona) = 0;
	virtual void Visit(Grass* floor_tile) = 0;

	virtual void Visit(Lumberjack* lumberjack) = 0;
	virtual void Visit(Worker* worker) = 0;

	virtual void Visit(Building* building) = 0;
	virtual void Visit(LumberjackHut* lumberjackHut) = 0;
	virtual void Visit(Dwelling* dwelling) = 0;
};
