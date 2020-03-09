#include "stdafx.h"
#pragma once
#include <building.h>
#include <grid.h>

Building::Building(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: GameObject(aPosition, aScale, aRotation) {};

void Building::CreateBuildingOutline()
{
	glm::vec3 scale = glm::vec3(0.0003f, 0.0003f, 0.0003f); //glm::vec3(0.012f, 0.006f, 0.012f);
	const glm::vec3 rotationZ = glm::vec3(0.0f, 0.0f, 1.0f); //glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 rotationX = glm::vec3(0.0f, 0.0f, 1.0f); //glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 rotationY = glm::vec3(0.0f, 1.0f, 0.0f); //glm::vec3(0.0f, 1.0f, 0.0f);

	constexpr float twoPi = glm::two_pi<float>();
	const float maxRotation = 0.1f;
	const float chanceForMoreRot = 0.04f;
	float moreRot = 0.5;
	float chance;

	for (int i = fromX; i <= toX; ++i)
	{
		// bottom right line where y = 0
		model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(i + 0.25f, fromY + 0.25f, position.z));
		model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
		model = glm::scale(model, scale);

		buildingOutlines.push_back(model);

		model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(i + 0.75f, fromY + 0.25f, position.z));
		model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
		model = glm::scale(model, scale);

		buildingOutlines.push_back(model);

		// top line where y = max
		model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(i + 0.25f, toY + 0.75f, position.z));
		model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
		model = glm::scale(model, scale);

		buildingOutlines.push_back(model);

		model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(i + 0.75f, toY + 0.75f, position.z));
		model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
		model = glm::scale(model, scale);

		buildingOutlines.push_back(model);
	}

	// left line where x = 0
	for (int i = fromY; i <= toY; ++i)
	{
		if (i != fromY)
		{
			model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(fromX + 0.25f, i + 0.25f, position.z));
			model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
			chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
			chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
			model = glm::scale(model, scale);

			buildingOutlines.push_back(model);

			model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(toX + 0.75f, i + 0.25f, position.z));
			model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
			chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
			chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
			model = glm::scale(model, scale);

			buildingOutlines.push_back(model);
		}
		if (i != toY)
		{
			model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(fromX + 0.25f, i + 0.75f, position.z));
			model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
			chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
			chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
			model = glm::scale(model, scale);

			buildingOutlines.push_back(model);

			model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(toX + 0.75f, i + 0.75, position.z));
			model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
			chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
			chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
			model = glm::scale(model, scale);

			buildingOutlines.push_back(model);
		}
	}
};
