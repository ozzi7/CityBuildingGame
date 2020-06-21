#pragma once
#include <game_object.h>
#include "visitor.h"
#include <math.h>
#include <vector>

class Grid;

/*	
	goingToWork : lumberjack has axe and walks to tree
	goingToWorkplace: worker walks to lumberjackhut 
	gettingResource : worker walks to resource to pick it up

	pending : waiting for game event to be processed
*/
enum class State { idle, pending, immigrating, carryingWood, carryingStone, gettingWood, gettingStone, goingToWork, goingToWorkplace, working, walking };

class BoneAnimated : public GameObject
{
public:
	BoneAnimated(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation);

	void UpdatePosition(Grid* grid);
	void SetNewPath(std::vector<std::pair<int,int>> aWayPoints);

	virtual void GameStep() = 0;
	bool visible = true;

	float animationSecond = 0.0f;

	GameObject* destination = nullptr;
	State state = State::idle;

protected:
	bool hasArrived = true;
	float walkingSpeed = 0.006f;
	float proxySpeed = 0.006f; // change this to increase walking speed!

private:

	bool proxyHasArrived = true;
	glm::vec2 proxyPosition = glm::vec2(0.0f, 0.0f); // TODO private

	std::vector<std::pair<int,int>> wayPoints;
	long long proxyWPIdx = 0;

	const float maxSpeed = 1.2f * proxySpeed;
	const float minSpeed = 0.7f * proxySpeed; // has to be low enough not to catch up to the proxy
	float maxAcceleration = proxySpeed / 500.0f; // /100
	float distanceToProxy = 0.0f;
	const float targetDistanceToProxy = 2.0f; // if higher the corners will be cut more, if lower then the speed up slow down is more visible

	void updateProxyPosition(float aDistance);
	void updateGridUnit();
};
