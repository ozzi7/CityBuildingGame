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

#include "noise_gen.h"
#include "shader.h"
#include "model.h"
#include "visitor.h"
#include "object.h"

class Terrain : Object
{
public:
	Terrain(int gridHeight, int gridWidth);
	~Terrain();

	void SetRenderWindow(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight);
	void Draw(Shader & shader);
	void LoadTextures(Shader *shaderTerrain, string exePath);
	void GenerateBuffers();
	void InitOpenGL(Shader *shaderTerrain, string exePath);
	void Accept(Visitor &v);

	vector<vector<float>> heightmap;

	/* Defines the size of renderData0, renderData1 (main memory buffers) in terms of tiles
	Change this to allow further zoom out */
	int maximumVisibleUnits = 600 * 500;

private:
	void CreateGeometry();
	void LoadVisibleGeometry(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight);
	void AddTexturesToGrid();
	void PopulateGridWithObjects();
	int ReloadGPUData();

	GLuint VBO, VAO, EBO;
	unsigned int texture_id_grass;
	string texture_grass = "Grass.bmp";

	int gridHeight;
	int gridWidth;

	vector<glm::vec3> vertexNormals;
	vector<glm::vec3> triangleNormals;
	vector<float> triangleArea;

	/* Defines what is rendered of the terrain*/
	int currUpperLeftX = 0;
	int currUpperLeftY = 0;
	int currLowerRightX = 0;
	int currLowerRightY = 0;

	mutex renderDataMutex;
	bool reloadGPUData = false;
	int currRenderData = 1;
	int renderDataVertexCount = 0;
	vector<GLfloat> *renderData0; /* Gets sent to GPU */
	vector<GLfloat> *renderData1; /* .. */
	vector<unsigned int> indices;
};