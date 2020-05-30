#include "stdafx.h"
#pragma once
#include <lumberjack_hut.h>

LumberjackHut::LumberjackHut(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation, float aFloorZ)
	: Building(aPosition, aScale, aRotation, aFloorZ) {
	workersRequired = 2;
	woodRequired = 3;
}

void LumberjackHut::Accept(Visitor& v)
{
	v.Visit(this);
}
void LumberjackHut::Evolve()
{
	evolutionStage++;

	scale = glm::vec3(0.025f, 0.025f, 0.025f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	ConsumeAllBuildingMaterial();

	recalculateModelMatix();
	UpdateWoodModels();
}
void LumberjackHut::UpdateWoodModels()
{
	woodModels.clear();

	glm::vec3 scale = glm::vec3(0.22f, 0.2f, 0.2f); //glm::vec3(0.012f, 0.006f, 0.012f);
	const glm::vec3 rotationZ = glm::vec3(0.0f, 0.0f, 1.0f); //glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 rotationX = glm::vec3(1.0f, 0.0f, 0.0f); //glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 rotationY = glm::vec3(0.0f, 1.0f, 0.0f); //glm::vec3(0.0f, 1.0f, 0.0f);

	int wood = std::max(this->wood, (int)this->woodStored);

	for (int i = 0; i < std::min(4, wood); ++i)
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = translate(model, glm::vec3(toX + (i-1) * 0.175f, posY + 0.45f, floorZ + 0.071f));

		model = rotate(model, glm::half_pi<float>(), rotationZ);
		model = glm::scale(model, scale);

		woodModels.push_back(model);
	}
	for (int i = 0; i < std::min(3, wood - 4); ++i)
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = translate(model, glm::vec3(toX + (i - 1) * 0.175f + 0.09f, posY + 0.45f, floorZ + 0.071f*3));

		model = rotate(model, glm::half_pi<float>(), rotationZ);
		model = glm::scale(model, scale);

		woodModels.push_back(model);
	}
	for (int i = 0; i < std::min(2, wood - 7); ++i)
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = translate(model, glm::vec3(toX + (i - 1) * 0.175f + 0.09f*2, posY + 0.45f, floorZ + 0.071f*5));

		model = rotate(model, glm::half_pi<float>(), rotationZ);
		model = glm::scale(model, scale);

		woodModels.push_back(model);
	}
	for (int i = 0; i < std::min(1, wood - 9); ++i)
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = translate(model, glm::vec3(toX + (i - 1) * 0.175f + 0.09f*3, posY + 0.45f, floorZ + 0.071f*7));

		model = rotate(model, glm::half_pi<float>(), rotationZ);
		model = glm::scale(model, scale);

		woodModels.push_back(model);
	}
}