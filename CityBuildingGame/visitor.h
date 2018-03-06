#pragma once

class Tree;
class WhiteTree;
class Fir;

class Visitor
{
public:
	virtual void Visit(Tree* tree) = 0;
	virtual void Visit(WhiteTree* white_tree) = 0;
	virtual void Visit(Fir* fir) = 0;
};