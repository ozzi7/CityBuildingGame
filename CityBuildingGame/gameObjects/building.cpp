#include "stdafx.h"
#pragma once
#include <building.h>
#include <grid.h>

Building::Building(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: GameObject(aPosition, aScale, aRotation) {};

void Building::CreateBuildingOutline()
{
	for (int i = fromX; i < toX; ++i)
	{
		// bottom right line where y = 0
		model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(i + 0.25f, fromY + 0.25f, position.z));
		model = rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.012f, 0.006f, 0.012f));

		buildingOutlines.push_back(model);

		model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(i + 0.75f, fromY + 0.25f, position.z));
		model = rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.012f, 0.006f, 0.012f));

		buildingOutlines.push_back(model);

		// top line where y = max
		model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(i + 0.25f, toY - 0.25f, position.z));
		model = rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.012f, 0.006f, 0.012f));

		buildingOutlines.push_back(model);

		model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(i + 0.75f, toY - 0.25f, position.z));
		model = rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.012f, 0.006f, 0.012f));

		buildingOutlines.push_back(model);
	}

	// left line where x = 0
	for (int i = fromY; i < toY; ++i)
	{
		if (i != fromY)
		{
			model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(fromX + 0.25f, i + 0.25f, position.z));
			model = rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.012f, 0.006f, 0.012f));

			buildingOutlines.push_back(model);

			model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(toX - 0.25f, i + 0.25f, position.z));
			model = rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.012f, 0.006f, 0.012f));

			buildingOutlines.push_back(model);
		}
		if (i != toY - 1)
		{
			model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(fromX + 0.25f, i + 0.75f, position.z));
			model = rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.012f, 0.006f, 0.012f));

			buildingOutlines.push_back(model);

			model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(toX - 0.25f, i + 0.75, position.z));
			model = rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.012f, 0.006f, 0.012f));

			buildingOutlines.push_back(model);
		}
	}
};
