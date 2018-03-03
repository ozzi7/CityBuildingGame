#pragma once

#include <vector>
#include <algorithm>
#include <time.h>
#include <mutex> 
#include <atomic>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

// Include GLM, math library
#include "glm/glm.hpp"
#include <math.h>
#include "glm/gtc/matrix_transform.hpp"

#include "heightmap.h"
#include "shader.h"
#include "model.h"

/* Represents a grid subunit */
class Unit
{
public:
	bool occupied = false;
	float averageHeight = 0;
	
	Unit(float pAverageHeight);
	~Unit();
};

class Terrain
{
public:
	Terrain();
	~Terrain();

	void Initialize(int argWidth, int argHeight);
	void SetRenderWindow(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight);
	float GetHeight(int argX, int argY);
	void Draw(Shader & shader);
	void LoadTextures(Shader & shaderTerrain, string exePath);
	void GenerateBuffers();
	void InitializeRenderData(int visibleWidth, int visibleHeight);

	vector<vector<Unit>> grid;
	vector<vector<float>> heightmap;

private:
	void CreateGeometry();
	void LoadVisibleGeometry(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight);
	void CreateGrid();
	void AddTexturesToGrid();
	void PopulateGridWithObjects();
	void ReloadGPUData();

	GLuint VBO, VAO, EBO;
	unsigned int texture_id_grass;
	string texture_grass = "Grass.bmp";

	int gridHeight;
	int gridWidth;

	vector<glm::vec3> vertexNormals;
	vector<glm::vec3> triangleNormals;
	vector<float> triangleArea;

	/* Defines what is rendered of the terrain*/
	int visibleHeight;
	int visibleWidth;
	int currUpperLeftX, currUpperLeftY = 0;

	mutex renderDataMutex;
	bool reloadGPUData = false;
	int currRenderData = 1;
	int renderDataVertexCount = 0;
	vector<GLfloat> *renderData0; /* Gets sent to GPU */
	vector<GLfloat> *renderData1; 
	vector<unsigned int> indices;
};