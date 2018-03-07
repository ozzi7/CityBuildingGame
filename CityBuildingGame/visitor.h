#pragma once

class Tree;
class Chamaecyparis;
class Fir;

class Visitor
{
public:
	virtual void Visit(Tree* tree) = 0;
	virtual void Visit(Chamaecyparis* white_tree) = 0;
	virtual void Visit(Fir* fir) = 0;
};