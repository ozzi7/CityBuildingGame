#pragma once

class Tree;
class Fir;
class Terrain;
class Lumberjack;

class Visitor
{
public:
	virtual void Visit(Tree* tree) = 0;
	virtual void Visit(Fir* fir) = 0;
	virtual void Visit(Terrain* terrain) = 0;
	virtual void Visit(Lumberjack* lumberjack) = 0;
};