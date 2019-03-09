#pragma once
#include <game_object.h>
#include "visitor.h"
#include <grid.h>
#include <math.h> 

class BoneAnimated : public GameObject
{
public:
	BoneAnimated(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: GameObject(aPosition, aScale, aRotation) {};

	void UpdatePosition(Grid * grid);
	void SetNewPath();

private:
	bool resetPath = true;

	int WPIdx = 0;
	int proxyWPIdx = 0;

	std::tuple<float> proxyObject;
	std::vector<glm::vec2> wayPoints;

	float walkingSpeed = 0.005f;
	float walkingSpeedMaxChange = walkingSpeed / 20.0f;

	void updateProxyPosition();
};