#include "stdafx.h"
#pragma once
#include <bone_animated.h>
#include <grid.h>

BoneAnimated::BoneAnimated(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: GameObject(aPosition, aScale, aRotation) {}

void BoneAnimated::UpdatePosition(Grid* grid)
{
	if (!proxyHasArrived)
		updateProxyPosition(proxySpeed*(1/grid->GetWalkingCost(wayPoints[proxyWPIdx].first, wayPoints[proxyWPIdx].second)));

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
			float translationX = walkingSpeed * glm::normalize(proxyPosition - glm::vec2(position.x, position.y)).x;
			float translationY = walkingSpeed * glm::normalize(proxyPosition - glm::vec2(position.x, position.y)).y;

			position = glm::vec3(position.x + translationX, position.y + translationY, grid->GetHeight(
				                     position.x + translationX,
				                     position.y + translationY));

			rotation.z = std::atan2f(translationY, translationX);
			if (translationX < 0)
				rotation.z += 2 * glm::pi<float>();
			rotation.z = rotation.z - glm::half_pi<float>(); // rotation direction is from +x to -y..
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
void BoneAnimated::updateProxyPosition(float aDistance)
{
	while (aDistance > 0.0f && !proxyHasArrived)
	{
		glm::vec2 translation = glm::vec2(0, 0);
		if (proxyWPIdx + 1 < wayPoints.size())
		{
			// not at final WP yet..
			glm::vec2 nextWayPointPosition = glm::vec2(wayPoints[proxyWPIdx + 1].first + 0.5f, wayPoints[proxyWPIdx + 1].second + 0.5f);
			float distanceToNextWP = distance(proxyPosition, nextWayPointPosition);
			
			glm::vec2 normalized = normalize(nextWayPointPosition - proxyPosition);
			if (glm::isnan(normalized.x) || glm::isnan(normalized.y))
			{
				// dont translate object but at least remove the distance (assume this is a normal value)
				aDistance -= distanceToNextWP;
			}
			else
			{
				translation = std::min(distanceToNextWP, aDistance) * normalize(nextWayPointPosition - proxyPosition);
				proxyPosition += translation;
				aDistance -= distanceToNextWP;
			}
			if (aDistance >= 0.0f)
			{
				// arrived at new waypoint if we need to push the object further than the next waypoint
				proxyWPIdx++;

				if (proxyWPIdx + 1 == wayPoints.size()) 
				{ 
					// arrived at final waypoint
					proxyHasArrived = true;
				}
			}
		}
		else
		{
			proxyHasArrived = true;
		}
	}
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
		updateProxyPosition(targetDistanceToProxy); // set initial proxy position
		hasArrived = false;
	}
	else
	{
		proxyHasArrived = true;
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
