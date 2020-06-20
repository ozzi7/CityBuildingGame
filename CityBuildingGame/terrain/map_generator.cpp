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
	generateGrass();
}
void MapGenerator::generateTerrain() const
{
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
		"Creating terrain..."));

	NoiseGen noise_gen;
	std::vector<std::vector<float>> heightmap = std::vector<std::vector<float>>(
		(long long)grid->gridHeight + 1, std::vector<float>((long long)grid->gridWidth + 1, 0));

	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
		"Generating perlin noise for terrain"));

	noise_gen.GeneratePerlinNoise(heightmap, grid->gridHeight + 1, grid->gridWidth + 1, -HILL_HEIGHT / 2.0f,
	                              HILL_HEIGHT / 2.0f, 6, PERSISTENCE);
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
		"Flattening map to create valleys and plateaus"));
	flattenMap(heightmap);

	grid->terrain->heightmap = heightmap;

	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
		"Creating geometry for the terrain"));
	grid->terrain->CreateGeometry();

	grid->Init();
}

void MapGenerator::generateTrees()
{
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
		"Creating trees..."));

	std::mt19937 gen(rd());
	std::chi_squared_distribution<> scale_tree(1.0f);
	std::uniform_real_distribution<> pos_offset_tree(-0.3f, 0.3f);
	std::uniform_real_distribution<> rotation(0, glm::two_pi<float>());

	/* create trees using noise */
	std::vector<std::vector<float>> treeMap = std::vector<std::vector<float>>(grid->gridHeight, std::vector<float>(grid->gridWidth, 0));

	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
		"Generating perlin noise for trees"));

	noiseGen.GeneratePerlinNoise(treeMap, grid->gridHeight, grid->gridWidth, 0.0f, 10.0f, 6, PERSISTENCE_TREES);

	/*Add terrain factor to the tree map*/
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
		"Combining terrain & tree noise to create tree distributions"));
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

	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
		"Planting trees"));

	for (int i = 0; i < grid->gridHeight; ++i)
	{
		for (int j = 0; j < grid->gridWidth; ++j)
		{
			float pine_prob = std::min(1.0f, PINE_DENSITY * getGaussianPDFValue(pine_mean, pine_var, treeMap[i][j]));
			float toona_prob = std::min(1.0f, TOONA_DENSITY * getGaussianPDFValue(toona_mean, toona_var, treeMap[i][j]));
			float euroBeech_prob = std::min(1.0f, EUROBEECH_DENSITY * getGaussianPDFValue(euroBeech_mean, euroBeech_var, treeMap[i][j]));
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
				grid->gridUnits[i][j].objects.push_back(
					new Pine(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
					         glm::vec3(scale * TREE_SCALE_FACTOR, scale * TREE_SCALE_FACTOR,
					                   scale * TREE_SCALE_FACTOR),
					         glm::vec3(glm::half_pi<float>(), 0, rotation(gen))));
			}
			else if (chosenTree <= 1 && isToona)
			{
				grid->gridUnits[i][j].objects.push_back(
					new Toona(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
					            glm::vec3(scale * TREE_SCALE_FACTOR, scale * TREE_SCALE_FACTOR,
					                      scale * TREE_SCALE_FACTOR),
					            glm::vec3(glm::half_pi<float>(), 0, rotation(gen))));
			}
			else if (chosenTree <= 2 && isEuroBeech)
			{
				float which_model_rand = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

				if (which_model_rand < 0.9f) {
					grid->gridUnits[i][j].objects.push_back(
						new EuroBeech(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
							glm::vec3(scale * TREE_SCALE_FACTOR, scale * TREE_SCALE_FACTOR,
								scale * TREE_SCALE_FACTOR),
							glm::vec3(glm::half_pi<float>(), 0, rotation(gen))));
				}
				else {
					grid->gridUnits[i][j].objects.push_back(
						new EuroBeech2(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
							glm::vec3(scale * TREE_SCALE_FACTOR, scale * TREE_SCALE_FACTOR,
								scale * TREE_SCALE_FACTOR),
							glm::vec3(glm::half_pi<float>(), 0, rotation(gen))));
				}
			}
			else if (chosenTree <= 3 && isOak)
			{
				grid->gridUnits[i][j].objects.push_back(
					new Oak(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
					        glm::vec3(scale * TREE_SCALE_FACTOR, scale * TREE_SCALE_FACTOR,
					                  scale * TREE_SCALE_FACTOR),
					        glm::vec3(glm::half_pi<float>(), 0, rotation(gen))));
			}
			grid->SetIsOccupied(j, i, true);
			grid->SetHasTree(j, i, true);
		}
	}
}

void MapGenerator::generateGrass()
{
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
		"Creating grass..."));

	std::mt19937 gen(rd());
	std::chi_squared_distribution<> scale_dist(1.0f);
	std::uniform_real_distribution<> position_offset(-0.4999f, 0.4999f);
	std::uniform_real_distribution<> z_rotation(0, glm::two_pi<float>());
	std::uniform_real_distribution<> y_rotation(0, glm::quarter_pi<float>()/4.0f);

	/* create grass using noise map*/
	std::vector<std::vector<float>> grassMap = std::vector<std::vector<float>>(grid->gridHeight, std::vector<float>(grid->gridWidth, 0));

	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
		"Generating perlin noise for grass"));

	noiseGen.GeneratePerlinNoise(grassMap, grid->gridHeight, grid->gridWidth, 0.0f, 10.0f, 6, PERSISTENCE_GRASS);

	/*Add terrain factor to the grass map*/
	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
		"Combining terrain & grass noise to create grass distributions"));
	for (int i = 0; i < grid->gridHeight; ++i)
	{
		for (int j = 0; j < grid->gridWidth; ++j)
		{
			grassMap[i][j] = grid->terrain->heightmap[i][j] * TERRAIN_WEIGHT_FACTOR_GRASS + grassMap[i][j] * (1 -
				TERRAIN_WEIGHT_FACTOR_GRASS);
		}
	}
	const float minHeight = getHeightAtPercentage(grassMap, 0.0f);
	const float maxHeight = getHeightAtPercentage(grassMap, 100.0f);

	const float grass_mean = getHeightAtPercentage(grassMap, GRASS_GAUSSIAN_MEAN_PERCENTAGE);

	const float grass_var = 0.01f * GRASS_GAUSSIAN_VARIANCE_PERCENTAGE * (maxHeight - minHeight);

	loggingEventHandler->AddEvent(new LoggingEvent(LoggingLevel::INFO, std::this_thread::get_id(), GetTickCount64(),
		"Planting grass"));

	for (int i = 0; i < grid->gridHeight; ++i)
	{
		for (int j = 0; j < grid->gridWidth; ++j)
		{
			float grass_prob = std::min(1.0f, GRASS_DENSITY * getGaussianPDFValue(grass_mean, grass_var, grassMap[i][j]));

			std::bernoulli_distribution grass_distribution(grass_prob);

			// get multiple samples per tile, always at least 20 grass if 
			if(grass_prob > 0.1f)
				for (int grassCount = 0; grassCount < 30; ++grassCount) // TODO: parameter max grass count
				{
					if (grassCount <= 10 || grass_distribution(gen))
					{
						float scale = 1.0f - (float)scale_dist(gen); // TODO
						while (scale < SMALL_GRASS_CUTOFF_PERCENTAGE * 0.01f)
						{
							scale = 1.0f - (float)scale_dist(gen);
						}

						float posX = j + 0.5f + (float)position_offset(gen);
						float posY = i + 0.5f + (float)position_offset(gen);

						// remove grass on the border of the map
						if (posX >= grid->gridWidth - 0.4f || posX <= 0.4f || posY >= grid->gridHeight - 0.4f || posY <= 0.4f)
							continue;

						if (grid->HasTree(j, i))
							continue;

						grid->gridUnits[i][j].objects.push_back(
							new Grass(glm::vec3(posX, posY, grid->GetHeight(posX, posY)),
								glm::vec3(scale * GRASS_SCALE_FACTOR, scale * GRASS_SCALE_FACTOR,
									scale * GRASS_SCALE_FACTOR),
								glm::vec3(0.0f, y_rotation(gen), z_rotation(gen))));
					}
				}
		}
	}
}

void MapGenerator::flattenMap(std::vector<std::vector<float>>& pHeightmap) const
{
	const float thresholdValley = getHeightAtPercentage(pHeightmap, VALLEY_PERCENTAGE);
	const float thresholdPlateau = getHeightAtPercentage(pHeightmap, 100 - PLATEAU_PERCENTAGE);

	/* cut off */
	for (std::vector<float>& i : pHeightmap)
		for (float& j : i)
		{
			if (j < thresholdValley)
				j = thresholdValley;
			
			else if (j > thresholdPlateau)
				j = thresholdPlateau;
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
		std::uniform_int_distribution<int> uni(0, pHeightmap.size()* pHeightmap[0].size()-1); // guaranteed unbiased

		for (int j = 0; j < maxNofSamples; j++)
		{
			auto random_integer = uni(rng);
			zValues.push_back(pHeightmap[random_integer / pHeightmap[0].size()][random_integer % pHeightmap[0].size()]);
		}
	}
	else 
		return getHeightAtPercentageExact(pHeightmap, percentage);

	std::sort(zValues.begin(), zValues.end());
	return zValues[(int)(0.01 * percentage * (zValues.size() - 1))];
}
/* Returns the height threshold where <percentage> of the map is lower */
float MapGenerator::getHeightAtPercentageExact(std::vector<std::vector<float>>& pHeightmap, float percentage) const
{
	if (percentage == 0.0f)
		return getMinValue(pHeightmap);
	else if (percentage == 100.0f)
		return getMaxValue(pHeightmap);

	std::vector<float> zValues;
	for (const std::vector<float>& i : pHeightmap)
		for (float j : i)
			zValues.push_back(j);

	std::sort(zValues.begin(), zValues.end());
	return zValues[(int)(0.01 * percentage * (zValues.size() - 1))];
}
// TODO: cache this or better yet save it when the map is generated (and flattened etc)
float MapGenerator::getMaxValue(std::vector<std::vector<float>>& pHeightmap)
{
	float maxValue = -FLT_MAX;
	for (std::vector<float>& i : pHeightmap)
		for (float j : i)
			if (j >= maxValue)
				maxValue = j;
	
	return maxValue;
}
// TODO: cache this or better yet save it when the map is generated (and flattened etc)
float MapGenerator::getMinValue(std::vector<std::vector<float>>& pHeightmap)
{
	float minValue = FLT_MAX;
	for (std::vector<float>& i : pHeightmap)
		for (float j : i)
			if (j <= minValue)
				minValue = j;
	
	return minValue;
}
float MapGenerator::getGaussianPDFValue(float mean, float var, float x)
{
	return 1 / std::sqrtf(2 * std::_Pi * var) * std::expf(-((x - mean) * (x - mean)) / (2 * var));
}
