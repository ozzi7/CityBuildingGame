#pragma once
#include "bone_animated.h"
#include "visitor.h"

class Lumberjack : public BoneAnimated
{
public:
	Lumberjack(glm::vec3 aPosition, glm::vec3 aScale, float aRotation)
		: BoneAnimated(aPosition, aScale, aRotation) {
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		model = glm::rotate(model, -1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
	};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};
};