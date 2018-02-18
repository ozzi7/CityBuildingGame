#pragma once
#include <vector>

using namespace std;

class Heightmap
{
public:
	Heightmap();
	~Heightmap();
	void GeneratePerlinNoise(vector<vector<float>>& pHeightmap, int pWidth, int pHeight,
		int maximumHeight, int octaveCount);

	void CombineNoiseMaps(vector<vector<float>> &pHeightmap,int octaveCount);

private:
	int width, height;

	float totalAmplitude;
	vector<vector<vector<float>>> smoothNoiseList; //an array of 2D arrays containing

	void GenerateWhiteNoise(vector<vector<float>> &pHeightmap);
	vector<vector<float>> GenerateSmoothNoise(vector<vector<float>> baseNoise, int octave);

	float Interpolate(float x0, float x1, float alpha);
	void Rescale(vector<vector<float>> &pHeightmap,float maxHeight);
};