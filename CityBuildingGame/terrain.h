# pragma once
#include <vector>
#include <algorithm>
#include <time.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

// Include GLM, math library
#include "glm/glm.hpp"
#include <math.h>
#include "glm/gtc/matrix_transform.hpp"

#include "common.h"

using std::vector;

class Terrain
{
public:
	Terrain();
	~Terrain();
	void Initialize(int argWidth, int argHeight);
	float Terrain::GetHeight(int argX, int argY);
	void Draw();


private:
	void GenerateWhiteNoise();
	vector<vector<float>> GenerateSmoothNoise(vector<vector<float>> baseNoise, int octave);
	void GeneratePerlinNoise(int octaveCount);
	float Interpolate(float x0, float x1, float alpha);

	void CreateGeometry();
	void LoadTextures();

	GLuint VBO, VAO, EBO;
	unsigned int texture1;
	std::string texture_grass_path = exe_path + "\\textures\\Grass.bmp";

	vector<vector<float>> heightMap;
	int height;
	int width;
};