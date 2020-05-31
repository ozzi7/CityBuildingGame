#include "stdafx.h"
#pragma once
#include <bone_animated.h>
#include <grid.h>

BoneAnimated::BoneAnimated(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: GameObject(aPosition, aScale, aRotation) {}

void BoneAnimated::UpdatePosition(Grid* grid)
{
	if (!proxyHasArrived)
		updateProxyPosition();

	if (!hasArrived)
	{
		distanceToProxy = distance(proxyPosition, glm::vec2(position.x, position.y));

		if (distanceToProxy < targetDistanceToProxy && walkingSpeed > minSpeed)
		{
			walkingSpeed -= maxAcceleration;
		}
		else if (distanceToProxy > targetDistanceToProxy&& walkingSpeed < maxSpeed)
		{
			walkingSpeed += maxAcceleration;
		}

		if (distanceToProxy > walkingSpeed)
		{
			// will not reach proxy yet
			float translationX = walkingSpeed * normalize(proxyPosition - glm::vec2(position.x, position.y)).x;
			float translationY = walkingSpeed * normalize(proxyPosition - glm::vec2(position.x, position.y)).y;

			position = glm::vec3(position.x + translationX, position.y + translationY, grid->GetHeight(
				                     position.x + translationX,
				                     position.y + translationY));

			rotation.z = std::atan2f(translationY, translationX);
			if (translationX < 0)
				rotation.z += 2 * glm::pi<float>();
			rotation.z = rotation.z - glm::half_pi<float>(); // rotation direction is fromt +x to -y..
		}
		else
		{
			hasArrived = true;

			position = glm::vec3(proxyPosition.x, proxyPosition.y,
			                     grid->GetHeight(proxyPosition.x, proxyPosition.y));
		}

		recalculateModelMatix();
		updateGridUnit();
	}
}
void BoneAnimated::updateProxyPosition()
{
	updateProxyPosition(walkingSpeed);
}
void BoneAnimated::updateProxyPosition(float aDistance)
{
	if (aDistance < 0.0f)
		aDistance = walkingSpeed;

	glm::vec2 translation = glm::vec2(0, 0);
	if (proxyWPIdx + 1 < wayPoints.size())
	{
		// not at final WP yet..
		float distanceToNextWP = distance(proxyPosition, glm::vec2(wayPoints[proxyWPIdx + 1].first + 0.5f, wayPoints[proxyWPIdx + 1].second + 0.5f));

		if (distanceToNextWP > aDistance)
		{
			// ..and will not arrive at a new WP this update
			translation = aDistance * normalize(glm::vec2(wayPoints[proxyWPIdx + 1].first + 0.5f, wayPoints[proxyWPIdx + 1].second + 0.5f) - proxyPosition);
		}
		else
		{
			// .. and will reach a new WP this update
			if (proxyWPIdx + 2 < wayPoints.size())
			{
				// continue to next WP after the one we just arrived at..
				translation = glm::vec2(wayPoints[proxyWPIdx + 1].first + 0.5f, wayPoints[proxyWPIdx + 1].second + 0.5f)
				              - proxyPosition + (aDistance - distanceToNextWP) *
							  normalize(glm::vec2(wayPoints[proxyWPIdx + 2].first + 0.5f, wayPoints[proxyWPIdx + 2].second + 0.5f) 
										- glm::vec2(wayPoints[proxyWPIdx + 1].first + 0.5f, wayPoints[proxyWPIdx + 1].second + 0.5f));
			}
			else
			{
				// ..unless we arrived at final WP
				translation = glm::vec2(wayPoints[proxyWPIdx + 1].first + 0.5f, wayPoints[proxyWPIdx + 1].second + 0.5f) - proxyPosition;
				proxyHasArrived = true;
			}
			proxyWPIdx++;
		}

		proxyPosition += translation;
	}
	else
		// arrived previously already
		proxyHasArrived = true;
}
/*NOTE: a path from x to y must contain x and y*/
void BoneAnimated::SetNewPath(std::vector<std::pair<int,int>> aWayPoints)
{
	wayPoints = aWayPoints;
	proxyWPIdx = 0;

	if (wayPoints.size() >= 1)
	{
		proxyPosition = glm::vec2(wayPoints[0].first + 0.5f, wayPoints[0].second + 0.5f);
		proxyHasArrived = false;
		updateProxyPosition(1.0f); // set initial proxy position
		hasArrived = false;
	}
	else
	{
		hasArrived = true;
	}
}

void BoneAnimated::updateGridUnit()
{
	if ((int)position.x != posX || (int)position.y != posY)
	{
		gameEventHandler->AddEvent(new MoveEvent(posX, posY, (int)position.x, (int)position.y, this));
		posX = (int)position.x;
		posY = (int)position.y;
	}
}
