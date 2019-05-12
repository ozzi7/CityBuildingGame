#pragma once
#include <vector>
#include <time.h>

class NoiseGen
{
public:
	NoiseGen();
	~NoiseGen();
	void GeneratePerlinNoise(std::vector<std::vector<float>>& pHeightmap, int pHeight, int pWidth,
	                         float minimumHeight, float maximumHeight, int octaveCount, float persistence);

	void CombineNoiseMaps(std::vector<std::vector<float>>& pHeightmap, int octaveCount, float aPersistence);

private:
	int width, height;

	float totalAmplitude;
	std::vector<std::vector<std::vector<float>>> smoothNoiseList; //an array of 2D arrays containing

	void GenerateWhiteNoise(std::vector<std::vector<float>>& pHeightmap) const;
	void GenerateSmoothNoise(std::vector<std::vector<float>>& baseNoise,
	                         std::vector<std::vector<float>>& smoothNoise, int octave);

	float Interpolate(float x0, float x1, float alpha);
	void Rescale(std::vector<std::vector<float>>& pHeightmap, float minHeight, float maxHeight) const;
};
