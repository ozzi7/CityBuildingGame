#include "stdafx.h"
#pragma once
#include <bone_animated.h>
#include <grid.h>

BoneAnimated::BoneAnimated(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: GameObject(aPosition, aScale, aRotation) {
}
void BoneAnimated::UpdatePosition(Grid * grid) 
{	
	if (!proxyHasArrived) {
		updateProxyPosition();
	}

	if (!hasArrived) {
		distanceToProxy = glm::distance(proxyObjectPos, glm::vec2(position.x, position.y));

		if (distanceToProxy < 1.0f && walkingSpeed > minSpeed) {
			walkingSpeed -= walkingSpeedMaxChange;
		}
		else if (distanceToProxy > 1.0f && walkingSpeed < maxSpeed)
		{	
			walkingSpeed += walkingSpeedMaxChange;
		}
		//if (previousDistanceToProxy < distanceToProxy && walkingSpeed < maxSpeed) {
		//	walkingSpeed += walkingSpeedMaxChange;
		//}
		/*else if(walkingSpeed > minSpeed)
		{
			walkingSpeed -= walkingSpeedMaxChange;
		}*/

		if (distanceToProxy > walkingSpeed) {
			// will not reach proxy yet
		}
		else {
			hasArrived = true;
			state = idle;
		}

		float translationX = walkingSpeed* glm::normalize(proxyObjectPos - glm::vec2(position.x, position.y)).x;
		float translationY = walkingSpeed* glm::normalize(proxyObjectPos - glm::vec2(position.x, position.y)).y;

		if (!std::isnan(translationX) && !std::isnan(translationY)) {
			position = glm::vec3(position.x + translationX, position.y + translationY, grid->GetHeight(position.x + translationX,
				position.y + translationY));

			rotation.z = std::atan2f(translationY, translationX);
			if (translationX < 0)
				rotation.z += 2 * glm::pi<float>();
			rotation.z = rotation.z - glm::half_pi<float>(); // somehow roation direction is not from +x to +y but +x to -y.. 
															 //rotation = rotation + 3.1415;
		}
		else {
			position = glm::vec3(proxyObjectPos.x, proxyObjectPos.y, grid->GetHeight(proxyObjectPos.x, proxyObjectPos.y));
		}

		recalculateModelMatix();
		updateGridUnit();
	}
}

void BoneAnimated::updateProxyPosition(float speed)
{
	if (speed < 0.0f) {
		speed = proxySpeed;
	}

	glm::vec2 translation = glm::vec2(0, 0);
	if (proxyWPIdx + 1 < wayPoints.size())
	{
		// not at final WP yet..
		float distanceToNextWP = glm::distance(proxyObjectPos, wayPoints[proxyWPIdx + 1]);
		
		if (distanceToNextWP > speed) {
			// ..and will not arrive at a new WP this update
			translation = speed*glm::normalize(wayPoints[proxyWPIdx + 1] - proxyObjectPos);
		}
		else {
			// .. and will reach a new WP this update
			if (proxyWPIdx + 2 < wayPoints.size())
			{
				// continue to next WP after the one we just arrived at..
				translation = (wayPoints[proxyWPIdx + 1] - proxyObjectPos) + (speed - distanceToNextWP)*
					glm::normalize(wayPoints[proxyWPIdx + 2] - wayPoints[proxyWPIdx + 1]);
			}
			else {
				// ..unless we arrived at final WP
				translation = (wayPoints[proxyWPIdx + 1] - proxyObjectPos);
				proxyHasArrived = true;
			}
			proxyWPIdx = proxyWPIdx + 1;
		}

		proxyObjectPos += translation;
	}
	else {
		// arrived previously already
		proxyHasArrived = true;
	}
}
/*NOTE: a path from x to y must contain x and y*/
void BoneAnimated::SetNewPath(std::vector<glm::vec2> aWayPoints)
{
	wayPoints = aWayPoints;

	proxyWPIdx = 0;

	if (wayPoints.size() == 1) {
		/* Can be used to set a position */
		proxyHasArrived = true;
		hasArrived = true;
		state = idle;
		proxyObjectPos = glm::vec2(wayPoints[0].x, wayPoints[0].y);
	}
	else if (wayPoints.size() >= 2) {
		proxyObjectPos = glm::vec2(wayPoints[0].x, wayPoints[0].y);
		proxyHasArrived = false;
		updateProxyPosition(1.0f); // set initial proxy position
		hasArrived = false;
		state = walking;
	}
}

void BoneAnimated::UpdatePath(std::vector<glm::vec2> aWayPoints)
{
	// TODO (if the path changes without having arrived yet..maybe we can merge it with setnewpath..)
}
void BoneAnimated::updateGridUnit()
{
	if ((int)position.x != posX || (int)position.y != posY) {
		unitEventHandler->AddEvent(new MoveEvent(posX, posY, (int)position.x, (int)position.y, this));
		posX = (int)position.x;
		posY = (int)position.y;
	}
}