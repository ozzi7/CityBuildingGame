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

	int x = 10;
	int y = 5;

	Lumberjack* lumbydumby = new Lumberjack(glm::vec3(0.5f, 0.5f, grid->gridUnits[0][0]->averageHeight),
		glm::vec3(0.0045f, 0.0045f, 0.0045f), glm::vec3(0, 0, glm::pi<float>()));
	
	Lumberjack* lumbydumbygoal = new Lumberjack(glm::vec3(x+0.5f, y+0.5f, grid->gridUnits[y][x]->averageHeight),
		glm::vec3(0.0045f, 0.0045f, 0.0045f), glm::vec3(0, 0, glm::pi<float>()));

	std::vector<glm::vec2> pathVector = std::vector<glm::vec2>();
	Pathfinding pathfinding = Pathfinding(grid, Coordinate(0, 0), Coordinate(5, 3));
	pathfinding.CalculatePath();
	std::list<Coordinate> path = pathfinding.GetPath();
	for (std::list<Coordinate>::iterator it = path.begin(); it != path.end(); ++it)
	{
		pathVector.push_back(glm::vec2((*it).first, (*it).second));
		std::cout << ' ' << (*it).first << '|' << (*it).second;
	}
	std::cout << '\n';

	lumbydumby->SetNewPath(pathVector);
	grid->gridUnits[0][0]->movingObjects.push_back(lumbydumby);
	grid->gridUnits[y][x]->objects.push_back(lumbydumbygoal);
}

void MapGenerator::generateTerrain()
{
	NoiseGen noise_gen;
	std::vector<std::vector<float>> heightmap = std::vector<std::vector<float>>(grid->gridHeight + 1, std::vector<float>(grid->gridWidth + 1, 0));
	noise_gen.GeneratePerlinNoise(heightmap, grid->gridHeight + 1, grid->gridWidth + 1, -HILL_HEIGHT/2.0f, HILL_HEIGHT/2.0f, 6, PERSISTENCE);
	flattenMap(heightmap);
		
	grid->terrain->heightmap = heightmap;
	grid->terrain->CreateGeometry();

	grid->Init();
}

void MapGenerator::generateTrees()
{
	std::mt19937 gen(rd());
	std::chi_squared_distribution<> scale_tree(1.0f);
	std::uniform_real_distribution<> pos_offset_tree(-0.3f, 0.3f);
	std::uniform_real_distribution<> rotation(0, glm::two_pi<float>());

	/* create trees using noise */
	treeMap = std::vector<std::vector<float>>(grid->gridHeight, std::vector<float>(grid->gridWidth, 0));
	noiseGen.GeneratePerlinNoise(treeMap, grid->gridHeight, grid->gridWidth, 0.0f, 10.0f, 6, PERSISTENCE_TREES);

	/*Add terrain factor to the tree map*/
	for (int i = 0; i < grid->gridHeight; ++i) {
		for (int j = 0; j < grid->gridWidth; ++j) {
			treeMap[i][j] = grid->terrain->heightmap[i][j]* TERRAIN_WEIGHT_FACTOR + treeMap[i][j]*(1-TERRAIN_WEIGHT_FACTOR);
		}
	}
	float minHeight = getHeightAtPercentage(treeMap, 0.0f);
	float maxHeight = getHeightAtPercentage(treeMap, 100.0f);

	float pine_mean = getHeightAtPercentage(treeMap, PINE_GAUSSIAN_MEAN_PERCENTAGE);
	float juniper_mean = getHeightAtPercentage(treeMap, JUNIPER_GAUSSIAN_MEAN_PERCENTAGE);
	float oak_mean = getHeightAtPercentage(treeMap, OAK_GAUSSIAN_MEAN_PERCENTAGE);
	float spruce_mean = getHeightAtPercentage(treeMap, SPRUCE_GAUSSIAN_MEAN_PERCENTAGE);

	float pine_var = 0.01f* PINE_GAUSSIAN_VARIANCE_PERCENTAGE*(maxHeight - minHeight);
	float juniper_var = 0.01f* JUNIPER_GAUSSIAN_VARIANCE_PERCENTAGE*(maxHeight - minHeight);
	float oak_var = 0.01f* OAK_GAUSSIAN_VARIANCE_PERCENTAGE*(maxHeight - minHeight);
	float spruce_var = 0.01f* SPRUCE_GAUSSIAN_VARIANCE_PERCENTAGE*(maxHeight - minHeight);

	for (int i = 0; i < grid->gridHeight; ++i) {
		for (int j = 0; j < grid->gridWidth; ++j) {

			float pine_prob = std::min(1.0f, PINE_DENSITY* getGaussianPDFValue(pine_mean, pine_var, treeMap[i][j]));
			float juniper_prob = std::min(1.0f, JUNIPER_DENSITY* getGaussianPDFValue(juniper_mean, juniper_var, treeMap[i][j]));
			float spruce_prob = std::min(1.0f, SPRUCE_DENSITY* getGaussianPDFValue(spruce_mean, spruce_var, treeMap[i][j]));
			float oak_prob = std::min(1.0f, OAK_DENSITY* getGaussianPDFValue(oak_mean, oak_var, treeMap[i][j]));

			std::bernoulli_distribution pine_distribution(pine_prob);
			bool isPine = pine_distribution(gen);
			std::bernoulli_distribution juniper_distribution(juniper_prob);
			bool isJuniper = juniper_distribution(gen);
			std::bernoulli_distribution spruce_distribution(spruce_prob);
			bool isSpruce = spruce_distribution(gen);
			std::bernoulli_distribution oak_distribution(oak_prob);
			bool isOak = oak_distribution(gen);

			int nofTies = isPine + isJuniper + isSpruce + isOak;

			if (nofTies == 0) continue;

			/* the uniform d. resolves ties if multiple trees would get generated at the same place*/
			std::uniform_int_distribution<int> dist(0, nofTies - 1);
			int chosenTree = dist(gen); 

			float scale = 1.0f - (float)scale_tree(gen);
			while (scale < SMALL_TREE_CUTOFF_PERCENTAGE*0.01f) { scale = 1.0f - (float)scale_tree(gen); }
			float posX = j + 0.5f + (float)pos_offset_tree(gen);
			float posY = i + 0.5f + (float)pos_offset_tree(gen);

			if (chosenTree == 0 && isPine) {
				grid->gridUnits[i][j]->objects.push_back(
					new Pine(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
						glm::vec3(scale*TREE_SCALE_FACTOR, scale*TREE_SCALE_FACTOR, scale*TREE_SCALE_FACTOR),
						glm::vec3(1.5707963f, 0, rotation(gen))));
				grid->gridUnits[i][j]->occupied = true;
			}
			else if (chosenTree <= 1 && isJuniper) {
				grid->gridUnits[i][j]->objects.push_back(
					new Juniper(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
						glm::vec3(scale*TREE_SCALE_FACTOR, scale*TREE_SCALE_FACTOR, scale*TREE_SCALE_FACTOR),
						glm::vec3(1.5707963f, 0, rotation(gen))));
				grid->gridUnits[i][j]->occupied = true;
			}
			else if (chosenTree <= 2 && isSpruce) {
				grid->gridUnits[i][j]->objects.push_back(
					new Spruce(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
						glm::vec3(scale*TREE_SCALE_FACTOR, scale*TREE_SCALE_FACTOR, scale*TREE_SCALE_FACTOR),
						glm::vec3(1.5707963f, 0, rotation(gen))));
				grid->gridUnits[i][j]->occupied = true;
			}
			else if (chosenTree <= 3 && isOak) {
				grid->gridUnits[i][j]->objects.push_back(
					new Oak(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
						glm::vec3(scale*TREE_SCALE_FACTOR, scale*TREE_SCALE_FACTOR, scale*TREE_SCALE_FACTOR),
						glm::vec3(1.5707963f, 0, rotation(gen))));
				grid->gridUnits[i][j]->occupied = true;
			}
			grid->gridUnits[i][j]->occupied = true;
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

float MapGenerator::getGaussianPDFValue(float mean, float var, float x)
{
	return (1 / std::sqrtf(2 * std::_Pi*var)*std::expf(-((x - mean)*(x - mean)) / (2 * var)));
}