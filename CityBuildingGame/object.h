#pragma once

class Object
{
public:
	virtual void Accept(class Visitor &v) = 0;
};