#pragma once
#include <game_object.h>
#include "visitor.h"
#include <math.h> 
#include <vector>

class Grid;

class BoneAnimated : public GameObject
{
public:
	BoneAnimated(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation);

	void UpdatePosition(Grid * grid);
	void SetNewPath(std::vector<glm::vec2> aWayPoints);
	void UpdatePath(std::vector<glm::vec2> aWayPoints);
	virtual void GameStep() = 0;
	bool visible = true;

	float animationSecond = 0.0f;

protected:
	bool hasArrived = true;
	int posX = 0;
	int posY = 0;
	float walkingSpeed = 0.006f;

private:

	bool proxyHasArrived = true;

	glm::vec2 proxyObjectPos = glm::vec2(0.0f,0.0f);
	std::vector<glm::vec2> wayPoints;
	int proxyWPIdx = 0;

	float proxySpeed = 0.006f;
	const float maxSpeed = 1.05*walkingSpeed;
	const float minSpeed = 0.7*walkingSpeed;
	float walkingSpeedMaxChange = walkingSpeed/500.0f; // /100
	float previousDistanceToProxy = 1.0f;
	float distanceToProxy = 1.0f;

	void updateProxyPosition(float speed = -1.0f);
	void updateGridUnit();
};