#pragma once
#include <bone_animated.h>

void BoneAnimated::UpdatePosition(Grid * grid)
{
	/*Check if we received a new path*/
	if (resetPath) {
		if (wayPoints.size() >= 2) {
			proxyObject = std::make_tuple(wayPoints[1].x, wayPoints[1].y);
			proxyWPIdx = 1;
		}
		return;
	}
	else {
		updateProxyPosition();
	}

}

void BoneAnimated::updateProxyPosition()
{
	glm::vec2 translation = glm::vec2(0, 0);
	if (WPIdx + 1 < wayPoints.size())
	{
		// has not arrived at last WP yet..
		float distanceToNextWP = glm::distance(glm::vec2(position.x, position.y), wayPoints[WPIdx + 1]);
		if (distanceToNextWP > walkingSpeed) {
			// will not arrive on next waypoint in this update..
			translation = walkingSpeed*glm::normalize(wayPoints[WPIdx + 1] - position);
		}
		else {
			// reached next waypoint, update index
			if (WPIdx + 2 < wayPoints.size())
			{
				translation = (wayPoints[WPIdx + 1] - position) + (walkingSpeed - distanceToNextWP)*
					glm::normalize(wayPoints[WPIdx + 2] - wayPoints[WPIdx + 1]);
			}
			else {
				// arrived in this update
				translation = (wayPoints[WPIdx + 1] - position);
			}
			WPIdx = WPIdx + 1;
		}

		position += translation;
		rotation.z = std::atan2f(translation.y, translation.x);
		if (translation.x < 0)
			rotation.z += 2 * 3.1415;
		rotation.z = rotation.z - 3.1415f / 2.0f; // somehow roation direction is not from +x to +y but +x to -y.. 
												  //rotation = rotation + 3.1415;
		recalculateModelMatix();
	}
	else {
		// arrived previously already
	}
}
/* TODO: Add new path as argument ? */
void BoneAnimated::SetNewPath()
{
	bool resetPath = true;
}
