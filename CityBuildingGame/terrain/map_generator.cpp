#include "map_generator.h"


MapGenerator::MapGenerator(Grid* aGrid)
{
	grid = aGrid;
}

void MapGenerator::GenerateMap() 
{
	//std::uniform_real_distribution<> scale_grass(0.9, 1.2);
	//std::uniform_real_distribution<> pos_offset_grass(-0.5, 0.5);

	generateTerrain();
	generateFirs();

	grid->gridUnits[0][0]->movingObjects.push_back(
		new Lumberjack(glm::vec3(0.5f, 0.5f, grid->gridUnits[0][0]->averageHeight),
			glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
}

void MapGenerator::generateTerrain()
{
	NoiseGen noise_gen;
	vector<vector<float>> heightmap = vector<vector<float>>(grid->gridHeight + 1, vector<float>(grid->gridWidth + 1, 0));
	noise_gen.GeneratePerlinNoise(heightmap, grid->gridHeight + 1, grid->gridWidth + 1, -HILL_HEIGHT * 0.7f, HILL_HEIGHT * 0.7f, 6);

	grid->terrain->heightmap = heightmap;
	grid->terrain->CreateGeometry();

	grid->Init();
}

void MapGenerator::generateFirs()
{
	std::mt19937 gen(rd());
	std::chi_squared_distribution<> scale_tree(1.0f);
	std::uniform_real_distribution<> pos_offset_tree(-0.4f, 0.4f);
	std::uniform_real_distribution<> rotation(0, glm::two_pi<float>());

	/* create trees using noise */
	treeMap = vector<vector<float>>(grid->gridHeight, vector<float>(grid->gridWidth, 0));
	noiseGen.GeneratePerlinNoise(treeMap, grid->gridHeight, grid->gridWidth, 0.0f, 5.0f + FIR_DENSITY, 3);

	for (int i = 0; i < grid->gridHeight; ++i) {
		for (int j = 0; j < grid->gridWidth; ++j) {
			if (treeMap[i][j] >= 5) {
				float scale = 1.0f - (float)scale_tree(gen);
				while (scale < 0.1f) {scale = 1.0f - (float)scale_tree(gen); }
				float posX = j + 0.5f + (float)pos_offset_tree(gen);
				float posY = i + 0.5f + (float)pos_offset_tree(gen);

				grid->gridUnits[i][j]->objects.push_back(
					new Fir(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
						glm::vec3(scale, scale, scale),
						glm::vec3(0, 0, rotation(gen))));
			}
		}
	}
}


