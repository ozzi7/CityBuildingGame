#include "stdafx.h"
#pragma once
#include <building.h>
#include <grid.h>


Building::Building(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation, int aGridX, int aGridY)
	: GameObject(aPosition, aScale, aRotation, aGridX, aGridY) {
};
void Building::createBuildingOutline()
{
	// position.z;  floor must be flat so this is correct height either way
	// TODO: simplify
	// bottom right line where y = 0
	for (int i = gridX - sizeX/2; i < sizeX + gridX - sizeX / 2; ++i)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(i + 0.25f, gridY + 0.25f - sizeY / 2, position.z));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); 
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, scale);

		buildingOutlines.push_back(model);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(i + 0.75, gridY + 0.25f - sizeY / 2, position.z));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, scale);

		buildingOutlines.push_back(model);
	}
	// top line where y = max
	for (int i = gridX - sizeX / 2; i < sizeX + gridX - sizeX / 2; ++i)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(i + 0.25f, gridY + sizeY - 0.25f - sizeY / 2, position.z));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, scale);

		buildingOutlines.push_back(model);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(i + 0.75f, gridY + sizeY - 0.25f - sizeY / 2, position.z));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, scale);

		buildingOutlines.push_back(model);
	}
	// left line where x = 0
	for (int i = gridY - sizeY / 2; i < sizeY + gridY - sizeY / 2; ++i)
	{
		if (i != gridY - sizeY / 2) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(gridX + 0.25f - sizeX / 2, i + 0.25f, position.z));
			model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, scale);

			buildingOutlines.push_back(model);
		}
		if (i != sizeY + gridY - 1 - sizeY / 2) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(gridX + 0.25f - sizeX / 2, i + 0.75f, position.z));
			model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, scale);
			buildingOutlines.push_back(model);
		}
	}
	// right line where x = max
	for (int i = gridY - sizeY / 2; i < sizeY + gridY - sizeY / 2; ++i)
	{
		if (i != gridY - sizeY / 2) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(gridX + sizeX - 0.25f - sizeX / 2, i + 0.25f, position.z));
			model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, scale);

			buildingOutlines.push_back(model);
		}
		if (i != sizeY + gridY - 1 - sizeY / 2) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(gridX + sizeX - 0.25f - sizeX / 2, i + 0.75, position.z));
			model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, scale);
			buildingOutlines.push_back(model);
		}
	}
};