#include "stdafx.h"
#pragma once
#include <dwelling.h>

Dwelling::Dwelling(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation, float aFloorZ)
	: Building(aPosition, aScale, aRotation, aFloorZ) {};

void Dwelling::Accept(Visitor& v)
{
	v.Visit(this);
};

void Dwelling::Evolve()
{
	evolutionStage++;

	scale = glm::vec3(0.18f, 0.18f, 0.18f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	recalculateModelMatix();
	UpdateWoodModels();
}

void Dwelling::UpdateWoodModels()
{
	woodModels.clear();

	glm::vec3 scale = glm::vec3(0.22f, 0.2f, 0.2f); //glm::vec3(0.012f, 0.006f, 0.012f);
	const glm::vec3 rotationZ = glm::vec3(0.0f, 0.0f, 1.0f); //glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 rotationX = glm::vec3(1.0f, 0.0f, 0.0f); //glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 rotationY = glm::vec3(0.0f, 1.0f, 0.0f); //glm::vec3(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < woodStored; ++i)
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = translate(model, glm::vec3(toX + (i + 2) * 0.175f, posY + 0.45f, floorZ + 0.075f));

		model = rotate(model, glm::half_pi<float>(), rotationZ);
		model = glm::scale(model, scale);

		woodModels.push_back(model);
	}
}

void Dwelling::AddWorker(unsigned int workers)
{
	workersPresent += workers;
	workersOnTheWay -= workers;
}

bool Dwelling::FreeWorkerCapacity() const
{
	if (workerCapacity - workersPresent - workersOnTheWay > 0)
		return true;
	return false;
}
