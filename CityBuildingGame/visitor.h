#pragma once

class Tree;
class Oak;
class Juniper;
class Spruce;
class Pine;
class Terrain;
class Lumberjack;

class Visitor
{
public:
	virtual void Visit(Tree* tree) = 0;
	virtual void Visit(Oak * oak) = 0;
	virtual void Visit(Spruce* spruce) = 0;
	virtual void Visit(Pine* pine) = 0;
	virtual void Visit(Juniper* juniper) = 0;
	virtual void Visit(Terrain* terrain) = 0;
	virtual void Visit(Lumberjack* lumberjack) = 0;
};