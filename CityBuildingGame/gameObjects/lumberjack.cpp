#pragma once
#include <lumberjack.h>

class Lumberjack : public BoneAnimated
{
public:
	Lumberjack(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: BoneAnimated(aPosition, aScale, aRotation) {
	};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};

private:

};