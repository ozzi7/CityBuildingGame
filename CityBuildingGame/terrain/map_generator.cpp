#include "stdafx.h"
#include "map_generator.h"


MapGenerator::MapGenerator(Grid* aGrid)
{
	grid = aGrid;
}

void MapGenerator::GenerateMap() 
{
	generateTerrain();
	generateTrees();

	grid->gridUnits[0][0]->movingObjects.push_back(
		new Lumberjack(glm::vec3(0.5f, 0.5f, grid->gridUnits[0][0]->averageHeight),
			glm::vec3(0.014f, 0.014f, 0.014f), glm::vec3(0, 0, 0)));
}

void MapGenerator::generateTerrain()
{
	NoiseGen noise_gen;
	std::vector<std::vector<float>> heightmap = std::vector<std::vector<float>>(grid->gridHeight + 1, std::vector<float>(grid->gridWidth + 1, 0));
	noise_gen.GeneratePerlinNoise(heightmap, grid->gridHeight + 1, grid->gridWidth + 1, 0.0f, HILL_HEIGHT, 6, PERSISTENCE);
	flattenMap(heightmap);
		
	grid->terrain->heightmap = heightmap;
	grid->terrain->CreateGeometry();

	grid->Init();
}

void MapGenerator::generateTrees()
{
	std::mt19937 gen(rd());
	std::chi_squared_distribution<> scale_tree(1.0f);
	std::uniform_real_distribution<> pos_offset_tree(-0.4f, 0.4f);
	std::uniform_real_distribution<> rotation(0, glm::two_pi<float>());

	/* create trees using noise */
	treeMap = std::vector<std::vector<float>>(grid->gridHeight, std::vector<float>(grid->gridWidth, 0));
	noiseGen.GeneratePerlinNoise(treeMap, grid->gridHeight, grid->gridWidth, 0.0f, 10.0f, 6, 0.7f);

	for (int i = 0; i < grid->gridHeight; ++i) {
		for (int j = 0; j < grid->gridWidth; ++j) {
			float scale = 1.0f - (float)scale_tree(gen);
			while (scale < 0.1f) { scale = 1.0f - (float)scale_tree(gen); }
			float posX = j + 0.5f + (float)pos_offset_tree(gen);
			float posY = i + 0.5f + (float)pos_offset_tree(gen);
			if (treeMap[i][j] >= 7) {
				grid->gridUnits[i][j]->objects.push_back(
					new Oak(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
						glm::vec3(scale*0.01f, scale*0.01f, scale*0.01f),
						glm::vec3(1.5707963f, 0, rotation(gen))));
			}
			else if (treeMap[i][j] >= 6) {
				grid->gridUnits[i][j]->objects.push_back(
					new Pine(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
						glm::vec3(scale*0.01f, scale*0.01f, scale*0.01f),
						glm::vec3(1.5707963f, 0, rotation(gen))));
			}
			else if (treeMap[i][j] >= 5) {
				grid->gridUnits[i][j]->objects.push_back(
					new Spruce(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
						glm::vec3(scale*0.01f, scale*0.01f, scale*0.01f),
						glm::vec3(1.5707963f, 0, rotation(gen))));
			}
			else if (treeMap[i][j] >= 3) {
				grid->gridUnits[i][j]->objects.push_back(
					new Juniper(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
						glm::vec3(scale*0.01f, scale*0.01f, scale*0.01f),
						glm::vec3(1.5707963f, 0, rotation(gen))));
			}
		}
	}
}

void MapGenerator::flattenMap(std::vector<std::vector<float>> &pHeightmap)
{
	float thresholdValley = getHeightAtPercentage(pHeightmap, VALLEY_PERCENTAGE);
	float thresholdPlateau = getHeightAtPercentage(pHeightmap, 100 - PLATEAU_PERCENTAGE);

	/* cut off */
	for (int i = 0; i < pHeightmap.size(); i++)
	{
		for (int j = 0; j < pHeightmap[i].size(); j++)
		{
			if (pHeightmap[i][j] < thresholdValley)
			{
				pHeightmap[i][j] = thresholdValley;
			}
			else if (pHeightmap[i][j] > thresholdPlateau)
			{
				pHeightmap[i][j] = thresholdPlateau;
			}
		}
	}
}
/* Returns the height threshold where <percentage> of the map is lower */
float MapGenerator::getHeightAtPercentage(std::vector<std::vector<float>> &pHeightmap, float percentage)
{
	std::vector<float> zValues;
	for (int i = 0; i < pHeightmap.size(); i++)
	{
		for (int j = 0; j < pHeightmap[i].size(); j++)
		{
			zValues.push_back(pHeightmap[i][j]);
		}
	}

	std::sort(zValues.begin(), zValues.end());
	return zValues[(int)(0.01* percentage*(zValues.size() - 1))];
}