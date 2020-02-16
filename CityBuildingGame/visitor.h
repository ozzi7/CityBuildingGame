#pragma once

class Tree;
class Oak;
class Toona;
class EuroBeech;
class Pine;
class Terrain;
class Lumberjack;
class Building;
class LumberjackHut;
class Dwelling;
class Settler;

class Visitor
{
public:
	virtual void Visit(Terrain* terrain) = 0;

	virtual void Visit(Tree* tree) = 0;
	virtual void Visit(Oak* oak) = 0;
	virtual void Visit(EuroBeech* euroBeech) = 0;
	virtual void Visit(Pine* pine) = 0;
	virtual void Visit(Toona* toona) = 0;

	virtual void Visit(Lumberjack* lumberjack) = 0;
	virtual void Visit(Settler* settler) = 0;

	virtual void Visit(Building* building) = 0;
	virtual void Visit(LumberjackHut* lumberjackHut) = 0;
	virtual void Visit(Dwelling* dwelling) = 0;
};
