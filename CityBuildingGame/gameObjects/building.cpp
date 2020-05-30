#include "stdafx.h"
#pragma once
#include <building.h>
#include <grid.h>

Building::Building(glm::vec3 aPosition, glm::vec3 aScale, glm::vec3 aRotation, float aFloorZ)
	: GameObject(aPosition, aScale, aRotation) 
{
	floorZ = aFloorZ;
};

void Building::CreateBuildingOutline()
{
	glm::vec3 scale = glm::vec3(0.0003f, 0.0003f, 0.0003f);
	const glm::vec3 rotationZ = glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 rotationX = glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 rotationY = glm::vec3(0.0f, 1.0f, 0.0f);

	constexpr float twoPi = glm::two_pi<float>();
	const float maxRotation = 0.1f;
	const float chanceForMoreRot = 0.04f;
	float moreRot = 0.5;
	float chance;

	for (int i = fromX; i <= toX; ++i)
	{
		// bottom right line where y = 0
		model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(i + 0.25f, fromY + 0.25f, floorZ));
		model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
		model = glm::scale(model, scale);

		buildingOutlines.push_back(model);

		model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(i + 0.75f, fromY + 0.25f, floorZ));
		model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
		model = glm::scale(model, scale);

		buildingOutlines.push_back(model);

		// top line where y = max
		model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(i + 0.25f, toY + 0.75f, floorZ));
		model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
		chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
		model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
		model = glm::scale(model, scale);

		buildingOutlines.push_back(model);

		model = glm::mat4(1.0f);
		model = translate(model, glm::vec3(i + 0.75f, toY + 0.75f, floorZ));
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
			model = translate(model, glm::vec3(fromX + 0.25f, i + 0.25f, floorZ));
			model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
			chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
			chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
			model = glm::scale(model, scale);

			buildingOutlines.push_back(model);

			model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(toX + 0.75f, i + 0.25f, floorZ));
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
			model = translate(model, glm::vec3(fromX + 0.25f, i + 0.75f, floorZ));
			model = rotate(model, rotation.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / twoPi)), rotationZ);
			chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			model = rotate(model, rotation.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationX);
			chance = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));
			model = rotate(model, rotation.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((chance <= chanceForMoreRot) ? moreRot : maxRotation))), rotationY);
			model = glm::scale(model, scale);

			buildingOutlines.push_back(model);

			model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(toX + 0.75f, i + 0.75, floorZ));
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
void Building::AddWoodBuildingMaterial(unsigned int wood)
{
	woodStored += wood;
	woodOnTheWay -= wood;

	if (AllRequiredBuildingMaterialsOnSite() && AllRequiredWorkersOnSite())
		Evolve();
}
void Building::AddStoneBuildingMaterial(unsigned int stone)
{
	stoneStored += stone;
	stoneOnTheWay -= stone;

	if (AllRequiredBuildingMaterialsOnSite() && AllRequiredWorkersOnSite())
		Evolve();
}

void Building::AddWorker(unsigned int workers)
{
	workersPresent += workers;
	workersOnTheWay -= workers;

	if (AllRequiredBuildingMaterialsOnSite() && AllRequiredWorkersOnSite())
		Evolve();
}

void Building::AddWoodBuildingMaterialOnTheWay(unsigned int wood)
{
	woodOnTheWay += wood;
}

void Building::AddStoneBuildingMaterialOnTheWay(unsigned int stone)
{
	stoneOnTheWay += stone;
}

void Building::AddWorkerOnTheWay(unsigned int workers)
{
	workersOnTheWay += workers;
}

void Building::ReserveWoodBuildingMaterial(unsigned int wood)
{
	woodReserved += wood;
}

void Building::ReserveStoneBuildingMaterial(unsigned int stone)
{
	stoneReserved += stone;
}

bool Building::RemoveWoodBuildingMaterial(unsigned int wood)
{
	if (woodStored >= wood)
	{
		woodReserved -= wood;
		woodStored -= wood;
		return true;
	}
	return false;
}

bool Building::RemoveStoneBuildingMaterial(unsigned int stone)
{
	if (stoneStored >= stone)
	{
		stoneReserved -= stone;
		stoneStored -= stone;
		return true;
	}
	return false;
}

bool Building::RemoveWorker(unsigned int workers)
{
	if (workersPresent >= workers)
	{
		workersPresent -= workers;
		return true;
	}
	return false;
}

void Building::ConsumeAllBuildingMaterial()
{
	if (woodStored > woodRequired)
	{
		woodStored -= woodRequired;
		woodRequired = 0;
	}
	else
	{
		woodRequired -= woodStored;
		woodStored = 0;
	}

	if (stoneStored > stoneRequired)
	{
		stoneStored -= stoneRequired;
		stoneRequired = 0;
	}
	else
	{
		stoneRequired -= stoneStored;
		stoneStored = 0;
	}
}

int Building::WoodBuildingMaterialRequired() const
{
	return (int)woodRequired - (int)woodStored - (int)woodOnTheWay;
}

int Building::StoneBuildingMaterialRequired() const
{
	return (int)stoneRequired - (int)stoneStored - (int)stoneOnTheWay;
}

int Building::WorkersRequired() const
{
	return (int)workersRequired - (int)workersPresent - (int)workersOnTheWay;
}

int Building::UnusedWoodBuildingMaterial() const
{
	return (int)woodStored - (int)woodReserved - (int)woodRequired;
}

int Building::UnusedStoneBuildingMaterial() const
{
	return (int)stoneStored - (int)stoneReserved - (int)stoneRequired;
}

int Building::UnusedWorkers() const

{
	return (int)workersPresent - (int)workersRequired;
}

bool Building::AllRequiredBuildingMaterialsOnSite() const
{
	if ((int)woodRequired - (int)woodStored <= 0 &&
		(int)stoneRequired - (int)stoneStored <= 0)
	{
		return true;
	}
	return false;
}

bool Building::AllRequiredBuildingMaterialsOnTheWay() const
{
	if (WoodBuildingMaterialRequired() <= 0 &&
		StoneBuildingMaterialRequired() <= 0)
	{
		return true;
	}
	return false;
}

bool Building::AllRequiredWorkersOnSite() const
{
	if ((int)workersRequired - (int)workersPresent <= 0)
		return true;
	return false;
}

bool Building::AllRequiredWorkersOnTheWay() const
{
	if (WorkersRequired() <= 0)
		return true;
	return false;
}
