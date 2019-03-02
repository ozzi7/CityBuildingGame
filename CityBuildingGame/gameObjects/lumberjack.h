#pragma once
#include "bone_animated.h"
#include <math.h> 
#include "visitor.h"

class Lumberjack : public BoneAnimated
{
public:
	Lumberjack(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: BoneAnimated(aPosition, aScale, aRotation) {
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));

		//wayPoints.push_back(glm::vec3(3.63833, 6.81779, 1.181271));
		//wayPoints.push_back(glm::vec3(3.86907, 6.58705, 1.230742));
		//wayPoints.push_back(glm::vec3(3.63388, 5.73592, 1.352878));
		//wayPoints.push_back(glm::vec3(3.55535, 4.78177, 1.477088));
		//wayPoints.push_back(glm::vec3(3.18281, 4.01433, 1.477088));
		//wayPoints.push_back(glm::vec3(2.8589, 3.22502, 1.489148));
		//wayPoints.push_back(glm::vec3(3.3160, 3.29101, 1.464005));
		//wayPoints.push_back(glm::vec3(3.86888, 3.44896, 1.464005));
		//wayPoints.push_back(glm::vec3(4.44139, 3.56043, 1.508691));
		//wayPoints.push_back(glm::vec3(4.84637, 3.82603, 1.508691));
	};
	void Accept(Visitor &v)
	{
		v.Visit(this);
	};
	/* Updates the position, rotation of the lumberjack*/
	void UpdatePosition()
	{
		/* TODO: temporary, assume pathfinding gives us a sequence of tiles*/
		// test: weigh direction by 1-distance to point 
		//glm::vec3 translation = glm::vec3();
		//if (wayPointIndex + 2 < wayPoints.size()) {
		//	float distance1 = glm::distance(position, wayPoints[wayPointIndex + 1]);
		//	float distance2 = glm::distance(position, wayPoints[wayPointIndex + 2])
		//	//model = glm::translate(wayPoints[wayPointIndex+1]-position)
		//}
		//
		//model = glm::translate(model, walkingSpeed*glm::vec3(wayPoints[next]);
		UpdatePositionPrimitive();
	}
	/* Updates the position without smoothing */
	void UpdatePositionPrimitive()
	{
		/* TODO: temporary, assume pathfinding gives us a sequence of x,y,z (unit middle)*/ 
		glm::vec3 translation = glm::vec3(0,0,0);
		if (wayPointIndex + 1 < wayPoints.size())
		{
			// not arrived yet
			float distanceToNextWP = glm::distance(position, wayPoints[wayPointIndex + 1]);
			if (distanceToNextWP > walkingSpeed) {
				// not on next waypoint yet
				translation = walkingSpeed*glm::normalize(wayPoints[wayPointIndex + 1] - position);
			}
			else {
				// reached next waypoint, update index
				if (wayPointIndex + 2 < wayPoints.size())
				{
					translation = (wayPoints[wayPointIndex + 1] - position) + (walkingSpeed - distanceToNextWP)*
						glm::normalize(wayPoints[wayPointIndex + 2] - wayPoints[wayPointIndex + 1]);
				}
				else {
					// arrived in this update
					translation = (wayPoints[wayPointIndex + 1] - position);
				}
				wayPointIndex = wayPointIndex + 1;
			}

			position += translation;
			rotation.z = std::atan2f(translation.y,translation.x);
			if (translation.x < 0)
				rotation += 2 * 3.1415;
			rotation = rotation - 3.1415f/2.0f; // somehow roation direction is not from +x to +y but +x to -y.. 
			//rotation = rotation + 3.1415;
			recalculateModelMatix();
		}
		else {
			// arrived previously already
		}

	}
	std::vector<glm::vec3> wayPoints; // TODO: for tests only, center point of grid units on path

private:
	float z = 0.0f;
	int wayPointIndex = 0;
	float walkingSpeed = 0.004f; // TODO: depends on model size.. 
	float maxRotation = 0.1f;
};