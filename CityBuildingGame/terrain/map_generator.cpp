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

	//int xStart = 30;
	//int yStart = 40;
	//int xDestination = 10;
	//int yDestination = 10;

	//Lumberjack* lumbydumby = new Lumberjack(glm::vec3(xStart + 0.5f, yStart + 0.5f, grid->gridUnits[yStart][xStart]->averageHeight),
	//	glm::vec3(0.0045f, 0.0045f, 0.0045f), glm::vec3(0, 0, glm::pi<float>()));

	//std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	//Pathfinding pathfinding = Pathfinding(grid, Coordinate(xStart, yStart), Coordinate(xDestination, yDestination));
	//pathfinding.CalculatePath();
	//std::list<Coordinate> path = pathfinding.GetPath();
	//std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	//std::chrono::microseconds total = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	//std::cout << "Path calculated and returned in " << total.count() << " microseconds \n";

	//std::vector<glm::vec2> pathVector = std::vector<glm::vec2>();
	//for (std::list<Coordinate>::iterator it = path.begin(); it != path.end(); ++it)
	//{
	//	pathVector.push_back(glm::vec2((*it).first, (*it).second));
	//	std::cout << ' ' << (*it).first << '|' << (*it).second;

	//	Lumberjack* pathLumby = new Lumberjack(glm::vec3((*it).first + 0.5f, (*it).second + 0.5f, grid->gridUnits[(*it).second][(*it).first]->averageHeight),
	//		glm::vec3(0.003f, 0.003f, 0.003f), glm::vec3(0, 0, glm::pi<float>()));
	//	grid->gridUnits[(*it).second][(*it).first]->movingObjects.push_back(pathLumby);
	//}
	//std::cout << '\n';

	////lumbydumby->SetNewPath(pathVector);
	//grid->gridUnits[yStart][xStart]->movingObjects.push_back(lumbydumby);

	/*int xStart = 30;
	int yStart = 40;

	Lumberjack* lumbydumby = new Lumberjack(glm::vec3(xStart + 0.5f, yStart + 0.5f, grid->gridUnits[yStart][xStart]->averageHeight),
		glm::vec3(0.0045f, 0.0045f, 0.0045f), glm::vec3(0, 0, glm::pi<float>()));

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	PathfindingObject pathfinding = PathfindingObject(grid, Coordinate(xStart, yStart));
	pathfinding.FindClosestTree();
	std::list<Coordinate> path = pathfinding.GetPath();

	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::chrono::microseconds total = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "Path calculated and returned in " << total.count() << " microseconds \n";

	std::vector<glm::vec2> pathVector = std::vector<glm::vec2>();
	for (std::list<Coordinate>::iterator it = path.begin(); it != path.end(); ++it)
	{
		pathVector.push_back(glm::vec2((*it).first, (*it).second));
		std::cout << ' ' << (*it).first << '|' << (*it).second;

		Lumberjack* pathLumby = new Lumberjack(glm::vec3((*it).first + 0.5f, (*it).second + 0.5f, grid->gridUnits[(*it).second][(*it).first]->averageHeight),
			glm::vec3(0.003f, 0.003f, 0.003f), glm::vec3(0, 0, glm::pi<float>()));
		grid->gridUnits[(*it).second][(*it).first]->movingObjects.push_back(pathLumby);
	}
	std::cout << '\n';

	lumbydumby->SetNewPath(pathVector);
	grid->gridUnits[yStart][xStart]->movingObjects.push_back(lumbydumby);*/
}

void MapGenerator::generateTerrain() const
{
	NoiseGen noise_gen;
	std::vector<std::vector<float>> heightmap = std::vector<std::vector<float>>(
		grid->gridHeight + 1, std::vector<float>(grid->gridWidth + 1, 0));
	noise_gen.GeneratePerlinNoise(heightmap, grid->gridHeight + 1, grid->gridWidth + 1, -HILL_HEIGHT / 2.0f,
	                              HILL_HEIGHT / 2.0f, 6, PERSISTENCE);
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
	for (int i = 0; i < grid->gridHeight; ++i)
	{
		for (int j = 0; j < grid->gridWidth; ++j)
		{
			treeMap[i][j] = grid->terrain->heightmap[i][j] * TERRAIN_WEIGHT_FACTOR + treeMap[i][j] * (1 -
				TERRAIN_WEIGHT_FACTOR);
		}
	}
	float minHeight = getHeightAtPercentage(treeMap, 0.0f);
	float maxHeight = getHeightAtPercentage(treeMap, 100.0f);

	float pine_mean = getHeightAtPercentage(treeMap, PINE_GAUSSIAN_MEAN_PERCENTAGE);
	float toona_mean = getHeightAtPercentage(treeMap, TOONA_GAUSSIAN_MEAN_PERCENTAGE);
	float oak_mean = getHeightAtPercentage(treeMap, OAK_GAUSSIAN_MEAN_PERCENTAGE);
	float euroBeech_mean = getHeightAtPercentage(treeMap, EUROBEECH_GAUSSIAN_MEAN_PERCENTAGE);

	float pine_var = 0.01f * PINE_GAUSSIAN_VARIANCE_PERCENTAGE * (maxHeight - minHeight);
	float toona_var = 0.01f * TOONA_GAUSSIAN_VARIANCE_PERCENTAGE * (maxHeight - minHeight);
	float oak_var = 0.01f * OAK_GAUSSIAN_VARIANCE_PERCENTAGE * (maxHeight - minHeight);
	float euroBeech_var = 0.01f * EUROBEECH_GAUSSIAN_VARIANCE_PERCENTAGE * (maxHeight - minHeight);

	for (int i = 0; i < grid->gridHeight; ++i)
	{
		for (int j = 0; j < grid->gridWidth; ++j)
		{
			float pine_prob = std::min(1.0f, PINE_DENSITY * getGaussianPDFValue(pine_mean, pine_var, treeMap[i][j]));
			float toona_prob = std::min(
				1.0f, TOONA_DENSITY * getGaussianPDFValue(toona_mean, toona_var, treeMap[i][j]));
			float euroBeech_prob = std::min(
				1.0f, EUROBEECH_DENSITY * getGaussianPDFValue(euroBeech_mean, euroBeech_var, treeMap[i][j]));
			float oak_prob = std::min(1.0f, OAK_DENSITY * getGaussianPDFValue(oak_mean, oak_var, treeMap[i][j]));

			std::bernoulli_distribution pine_distribution(pine_prob);
			bool isPine = pine_distribution(gen);
			std::bernoulli_distribution toona_distribution(toona_prob);
			bool isToona = toona_distribution(gen);
			std::bernoulli_distribution euroBeech_distribution(euroBeech_prob);
			bool isEuroBeech = euroBeech_distribution(gen);
			std::bernoulli_distribution oak_distribution(oak_prob);
			bool isOak = oak_distribution(gen);

			int nofTies = isPine + isToona + isEuroBeech + isOak;

			if (nofTies == 0) continue;

			/* the uniform d. resolves ties if multiple trees would get generated at the same place*/
			std::uniform_int_distribution<int> dist(0, nofTies - 1);
			int chosenTree = dist(gen);

			float scale = 1.0f - (float)scale_tree(gen);
			while (scale < SMALL_TREE_CUTOFF_PERCENTAGE * 0.01f)
			{
				scale = 1.0f - (float)scale_tree(gen);
			}
			float posX = j + 0.5f + (float)pos_offset_tree(gen);
			float posY = i + 0.5f + (float)pos_offset_tree(gen);

			if (chosenTree == 0 && isPine)
			{
				grid->gridUnits[i][j]->objects.push_back(
					new Pine(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
					         glm::vec3(scale * TREE_SCALE_FACTOR, scale * TREE_SCALE_FACTOR,
					                   scale * TREE_SCALE_FACTOR),
					         glm::vec3(1.5707963f, 0, rotation(gen))));
			}
			else if (chosenTree <= 1 && isToona)
			{
				grid->gridUnits[i][j]->objects.push_back(
					new Toona(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
					            glm::vec3(scale * TREE_SCALE_FACTOR, scale * TREE_SCALE_FACTOR,
					                      scale * TREE_SCALE_FACTOR),
					            glm::vec3(1.5707963f, 0, rotation(gen))));
			}
			else if (chosenTree <= 2 && isEuroBeech)
			{
				float which_model_rand = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

				if (which_model_rand < 0.5f) {
					grid->gridUnits[i][j]->objects.push_back(
						new EuroBeech(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
							glm::vec3(scale * TREE_SCALE_FACTOR, scale * TREE_SCALE_FACTOR,
								scale * TREE_SCALE_FACTOR),
							glm::vec3(1.5707963f, 0, rotation(gen))));
				}
				else {
					grid->gridUnits[i][j]->objects.push_back(
						new EuroBeech2(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
							glm::vec3(scale * TREE_SCALE_FACTOR, scale * TREE_SCALE_FACTOR,
								scale * TREE_SCALE_FACTOR),
							glm::vec3(1.5707963f, 0, rotation(gen))));
				}
			}
			else if (chosenTree <= 3 && isOak)
			{
				grid->gridUnits[i][j]->objects.push_back(
					new Oak(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
					        glm::vec3(scale * TREE_SCALE_FACTOR, scale * TREE_SCALE_FACTOR,
					                  scale * TREE_SCALE_FACTOR),
					        glm::vec3(1.5707963f, 0, rotation(gen))));
			}
			grid->gridUnits[i][j]->occupied = true;
			grid->gridUnits[i][j]->hasTree = true;
		}
	}
}

void MapGenerator::flattenMap(std::vector<std::vector<float>>& pHeightmap) const
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
/*  
	Returns the height threshold where <percentage> of the map is lower
	Don't sort the whole map to get this value if it is too big, simply take a sample from it 
*/
float MapGenerator::getHeightAtPercentage(std::vector<std::vector<float>>& pHeightmap, float percentage) const
{
	const int maxNofSamples = 1000; // 1000 random samples are enough to estimate this 
	std::vector<float> zValues;

	if (pHeightmap.size()* pHeightmap[0].size() > maxNofSamples && percentage != 0.0f && percentage != 100.0f) {

		std::random_device rd;     // only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(0, pHeightmap.size()* pHeightmap[0].size()); // guaranteed unbiased

		for (int j = 0; j < maxNofSamples; j++)
		{
			auto random_integer = uni(rng);
			zValues.push_back(pHeightmap[random_integer% pHeightmap.size()][random_integer / pHeightmap[0].size()]);
		}
	}
	else 
	{
		return getHeightAtPercentageExact(pHeightmap, percentage);
	}

	std::sort(zValues.begin(), zValues.end());
	return zValues[(int)(0.01 * percentage * (zValues.size() - 1))];
}
/* Returns the height threshold where <percentage> of the map is lower */
float MapGenerator::getHeightAtPercentageExact(std::vector<std::vector<float>>& pHeightmap, float percentage) const
{
	if (percentage == 0.0f)
	{
		return getMinValue(pHeightmap);
	}
	else if (percentage == 100.0f)
		return getMaxValue(pHeightmap);

	std::vector<float> zValues;
	for (int i = 0; i < pHeightmap.size(); i++)
	{
		for (int j = 0; j < pHeightmap[i].size(); j++)
		{
			zValues.push_back(pHeightmap[i][j]);
		}
	}

	std::sort(zValues.begin(), zValues.end());
	return zValues[(int)(0.01 * percentage * (zValues.size() - 1))];
}
// TODO: cache this or better yet save it when the map is generated (and flattened etc)
float MapGenerator::getMaxValue(std::vector<std::vector<float>>& pHeightmap) const
{
	float maxValue = -FLT_MAX;
	for (int i = 0; i < pHeightmap.size(); i++)
	{
		for (int j = 0; j < pHeightmap[i].size(); j++)
		{
			if (pHeightmap[i][j] >= maxValue)
				maxValue = pHeightmap[i][j];
		}
	}
	return maxValue;
}
// TODO: cache this or better yet save it when the map is generated (and flattened etc)
float MapGenerator::getMinValue(std::vector<std::vector<float>>& pHeightmap) const
{
	float minValue = FLT_MAX;
	for (int i = 0; i < pHeightmap.size(); i++)
	{
		for (int j = 0; j < pHeightmap[i].size(); j++)
		{
			if (pHeightmap[i][j] <= minValue)
				minValue = pHeightmap[i][j];
		}
	}
	return minValue;
}
float MapGenerator::getGaussianPDFValue(float mean, float var, float x)
{
	return 1 / std::sqrtf(2 * std::_Pi * var) * std::expf(-((x - mean) * (x - mean)) / (2 * var));
}
