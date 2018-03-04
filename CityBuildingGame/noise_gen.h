#pragma once
#include <vector>
#include <time.h>

using namespace std;

class NoiseGen
{
public:
	NoiseGen();
	~NoiseGen();
	void GeneratePerlinNoise(vector<vector<float>>& pHeightmap, int pHeight, int pWidth,
		int maximumHeight, int octaveCount);

	void CombineNoiseMaps(vector<vector<float>> &pHeightmap,int octaveCount);

private:
	int width, height;

	float totalAmplitude;
	vector<vector<vector<float>>> smoothNoiseList; //an array of 2D arrays containing

	void GenerateWhiteNoise(vector<vector<float>> &pHeightmap);
	void GenerateSmoothNoise(vector<vector<float>> &baseNoise,
		vector<vector<float>> &smoothNoise, int octave);

	float Interpolate(float x0, float x1, float alpha);
	void Rescale(vector<vector<float>> &pHeightmap,float maxHeight);
};