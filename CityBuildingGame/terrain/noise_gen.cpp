#include "stdafx.h"
#pragma once
#include "terrain.h"

NoiseGen::NoiseGen() {};

void NoiseGen::GeneratePerlinNoise(std::vector<std::vector<float>>& pHeightmap, int pHeight, int pWidth,
                                   float minimumHeight, float maximumHeight, int octaveCount, float persistence)
{
	width = pWidth;
	height = pHeight;

	GenerateWhiteNoise(pHeightmap);

	GenerateSmoothNoise(pHeightmap, octaveCount);

	CombineNoiseMaps(pHeightmap, octaveCount, persistence);

	Rescale(pHeightmap, minimumHeight, maximumHeight);
}

void NoiseGen::CombineNoiseMaps(std::vector<std::vector<float>>& pHeightmap, int octaveCount, float aPersistence)
{
	float persistence = aPersistence;
	float amplitude = 1.0f;

	//blend noise together
	for (auto& i : pHeightmap)
		fill(i.begin(), i.end(), 0);

	totalAmplitude = 0.0f;

	float maxValue = 0;
	for (int octave = octaveCount - 1; octave >= 0; octave--)
	{
		amplitude *= persistence;
		totalAmplitude += amplitude;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				pHeightmap[i][j] += smoothNoiseList[octave][i][j] * amplitude;
			}
		}
	}
}

void NoiseGen::GenerateWhiteNoise(std::vector<std::vector<float>>& pHeightmap) const
{
	srand((unsigned)time(nullptr)); // seed random numbers

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pHeightmap[i][j] = (float)rand() / (float)RAND_MAX; // generate random numbers between 0 and 1
		}
	}
}

void NoiseGen::GenerateSmoothNoise(std::vector<std::vector<float>>& baseNoise, int nofOctaves)
{
	smoothNoiseList = std::vector<std::vector<std::vector<float>>>(
		nofOctaves, std::vector<std::vector<float>>(height, std::vector<float>(width, 0.0f)));
	std::vector<int> samplePeriods = std::vector<int>(nofOctaves);
	std::vector<float> sampleFrequencies = std::vector<float>(nofOctaves);
	std::vector<int> sample_i0s = std::vector<int>(nofOctaves);
	std::vector<int> sample_i1s = std::vector<int>(nofOctaves);
	std::vector<int> sample_j0s = std::vector<int>(nofOctaves);
	std::vector<int> sample_j1s = std::vector<int>(nofOctaves);	
	std::vector<float> vertical_blends = std::vector<float>(nofOctaves);

	for (int k = 0; k < nofOctaves; ++k)
	{
		samplePeriods[k] = 1 << k;
		sampleFrequencies[k] = 1.0f / samplePeriods[k];
	}

	for (int i = 0; i < height; i++)
	{
		//calculate the horizontal sampling indices
		for (int k = 0; k < nofOctaves; ++k)
		{
			sample_i0s[k] = i / samplePeriods[k] * samplePeriods[k];
			sample_i1s[k] = (sample_i0s[k] + samplePeriods[k]) % height;
			vertical_blends[k] = (i - sample_i0s[k]) * sampleFrequencies[k];
		}

		for (int j = 0; j < width; j++)
		{
			//calculate the vertical sampling indices
			for (int k = 0; k < nofOctaves; ++k)
			{
				sample_j0s[k] = j / samplePeriods[k] * samplePeriods[k];
				sample_j1s[k] = (sample_j0s[k] + samplePeriods[k]) % width; //wrap around
				float horizontal_blend = (j - sample_j0s[k]) * sampleFrequencies[k];

				//blend the top two corners
				float top = Interpolate(baseNoise[sample_i0s[k]][sample_j0s[k]],
					baseNoise[sample_i1s[k]][sample_j0s[k]], vertical_blends[k]);

				//blend the bottom two corners
				float bottom = Interpolate(baseNoise[sample_i0s[k]][sample_j1s[k]],
					baseNoise[sample_i1s[k]][sample_j1s[k]], vertical_blends[k]);

				//final blend
				smoothNoiseList[k][i][j] = Interpolate(top, bottom, horizontal_blend);
			}
		}
	}
}

float NoiseGen::Interpolate(float x0, float x1, float alpha)
{
	return x0 * (1 - alpha) + alpha * x1;
}

void NoiseGen::Rescale(std::vector<std::vector<float>>& pHeightmap, float minHeight, float maxHeight) const
{
	//normalisation
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pHeightmap[i][j] = pHeightmap[i][j] * ((maxHeight - minHeight) / totalAmplitude) + minHeight;
		}
	}
}

NoiseGen::~NoiseGen() {};
