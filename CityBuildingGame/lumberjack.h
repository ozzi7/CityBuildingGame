#pragma once
#include "bone_animated.h"
#include "visitor.h"

class Lumberjack : public BoneAnimated
{
public:
	Lumberjack(glm::vec3 aPosition, glm::vec3 aScale, float aRotation)
		: BoneAnimated(aPosition, aScale, aRotation) {
		model = glm::translate(model, position);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.1f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::rotate(model, -3.1416f, glm::vec3(1.0f, 0.0f, 0.0f));
	};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};
	/* Updates the position, rotation of the lumberjack*/
	void UpdatePosition()
	{
		/* TODO: temporary, assume pathfinding gives us a sequence of tiles (which can be visited
		without obstacles in the direct line*/
	}
private:
	float walkingSpeed = 0.1f;
	float maxRotation = 0.1;
	vector<tuple<float, float>> wayPoints; // TODO: for tests only
};