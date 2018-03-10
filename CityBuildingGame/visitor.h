#pragma once

class Tree;
class Chamaecyparis;
class Fir;
class Palm;

class Visitor
{
public:
	virtual void Visit(Tree* tree) = 0;
	virtual void Visit(Chamaecyparis* white_tree) = 0;
	virtual void Visit(Fir* fir) = 0;
	virtual void Visit(Palm* palm) = 0;
};