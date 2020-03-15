#include "stdafx.h"
#pragma once
#include <lumberjack_hut.h>

LumberjackHut::LumberjackHut(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation)
	: Building(aPosition, aScale, aRotation) {
	requiredWorkers = 2;
};

void LumberjackHut::Accept(Visitor& v)
{
	v.Visit(this);
}
void LumberjackHut::Evolve()
{
	evolutionStage++;

	scale = glm::vec3(0.025f, 0.025f, 0.025f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	recalculateModelMatix();
	UpdateWoodModels();
}
void LumberjackHut::UpdateWoodModels()
{
	woodModels.clear();

	glm::vec3 scale = glm::vec3(0.3f, 0.3f,0.3f); //glm::vec3(0.012f, 0.006f, 0.012f);
	const glm::vec3 rotationZ = glm::vec3(0.0f, 0.0f, 1.0f); //glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 rotationX = glm::vec3(0.0f, 0.0f, 1.0f); //glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 rotationY = glm::vec3(0.0f, 1.0f, 0.0f); //glm::vec3(0.0f, 1.0f, 0.0f);

	for (int i = 0; i <= 9; ++i)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(posX + 2.25f, posY + 2.25f + i * 0.25f, position.z));
		model = glm::scale(model, scale);

		woodModels.push_back(model);
	}
}