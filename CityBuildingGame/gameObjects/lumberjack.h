#pragma once
#include "gameObjects/bone_animated.h"
#include <math.h> 

class Lumberjack : public BoneAnimated
{
public:
	Lumberjack(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
		: BoneAnimated(aPosition, aScale, aRotation) {
		model = glm::translate(model, position);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		//model = glm::rotate(model, -3.1416f, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, -1.5708f, glm::vec3(0.0f, 0.0f, 1.0f));

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
	vector<glm::vec3> wayPoints; // TODO: for tests only, center point of grid units on path

private:
	float z = 0.0f;
	int wayPointIndex = 0;
	float walkingSpeed = 0.004f; // TODO: depends on model size.. 
	float maxRotation = 0.1f;
	/*
	(x | y) (3.68684 | 7.4935)
		(x | y) adjusted(3.86812 | 7.31223) by height z 0.181271
		(x | y) (3.63833 | 6.81779)
		(x | y) adjusted(3.86907 | 6.58705) by height z 0.230742
		(x | y) (3.281 | 6.08879)
		Adjusted grid unit
		(x | y) adjusted(3.63388 | 5.73592) by height z 0.352878
		(x | y) (3.07826 | 5.25886)
		Adjusted grid unit
		(x | y) adjusted(3.55535 | 4.78177) by height z 0.477088
		(x | y) (2.70572 | 4.49142)
		Adjusted grid unit
		(x | y) adjusted(3.18281 | 4.01433) by height z 0.477088
		(x | y) (2.36982 | 3.71417)
		(x | y) adjusted(2.85897 | 3.22502) by height z 0.489148
		(x | y) (2.85202 | 3.75501)
		Adjusted grid unit
		(x | y) adjusted(3.31603 | 3.29101) by height z 0.464005
		(x | y) (3.40488 | 3.91297)
		(x | y) adjusted(3.86888 | 3.44896) by height z 0.464005
		(x | y) (3.9327 | 4.06912)
		Adjusted grid unit
		(x | y) adjusted(4.44139 | 3.56043) by height z 0.508691
		(x | y) (4.33768 | 4.33472)
		Adjusted grid unit
		(x | y) adjusted(4.84637 | 3.82603) by height z 0.508691
		(x | y) (4.80073 | 4.54225)
		Adjusted grid unit
		(x | y) adjusted(5.14815 | 4.19483) by height z 0.347421
		(x | y) (4.68795 | 5.31218)
		Adjusted grid unit
		(x | y) adjusted(5.03537 | 4.96476) by height z 0.347421
		(x | y) (4.72485 | 5.9995)
		Adjusted grid unit
		(x | y) adjusted(4.93031 | 5.79405) by height z 0.205458
		(x | y) (5.14275 | 6.18512)
		(x | y) adjusted(5.2451 | 6.08277) by height z 0.102347
		(x | y) (5.6701 | 6.24787)
		(x | y) adjusted(5.77245 | 6.14553) by height z 0.102347
		(x | y) (5.62158 | 5.57217)
		(x | y) adjusted(5.82704 | 5.36671) by height z 0.205458
		(x | y) (6.02069 | 5.66929)
		(x | y) adjusted(6.10277 | 5.58721) by height z 0.0820849
		(x | y) (6.6097 | 5.72403)
		(x | y) adjusted(6.69178 | 5.64194) by height z 0.0820849
		(x | y) (6.2962 | 5.08532)
		(x | y) adjusted(6.37828 | 5.00323) by height z 0.0820849
		(x | y) (6.85576 | 5.24998)
		(x | y) adjusted(6.93784 | 5.16789) by height z 0.0820849
		(x | y) (6.53064 | 4.62288)
		(x | y) adjusted(6.73398 | 4.41954) by height z 0.20334
		(x | y) (7.00303 | 4.70036)
		(x | y) adjusted(7.09476 | 4.60863) by height z 0.0917388
		(x | y) (7.63359 | 4.72696)
		(x | y) adjusted(7.72533 | 4.63523) by height z 0.0917388
		(x | y) (7.74341 | 5.64982)
		(x | y) adjusted(7.73213 | 5.6611) by height z - 0.0112849
		(x | y) (7.5315 | 6.59937)
		(x | y) adjusted(7.44952 | 6.68134) by height z - 0.0819757
		(x | y) (7.23516 | 7.67356)
		(x | y) adjusted(7.11335 | 7.79538) by height z - 0.121812
		(x | y) (6.9696 | 8.56947)
		(x | y) adjusted(6.87025 | 8.66881) by height z - 0.0993465*/
};