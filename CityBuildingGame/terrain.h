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

#include "common.h"
#include "heightmap.h"
#include "game_class.h"
#include "shader.h"

using namespace std;

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
	void Update();
	float GetHeight(int argX, int argY);
	void Draw();

	vector<vector<Unit>> grid;
	vector<vector<float>> heightmap;

private:
	void CreateGeometry();
	void LoadVisibleGeometry();
	void LoadVisibleGeometry(int startX, int endX, int startY, int endY);
	void LoadTextures();
	void CreateGrid();
	void AddTexturesToGrid();
	void PopulateGridWithObjects();
	void ReloadGPUData();
	void GenerateBuffers();

	GLuint VBO, VAO, EBO;
	unsigned int texture_id_grass;
	string texture_grass = "Grass.bmp";
	Shader terrainShader = Shader("basic_lighting.vert", "basic_lighting.frag");
	
	int gridHeight;
	int gridWidth;

	vector<glm::vec3> vertexNormals;
	vector<glm::vec3> triangleNormals;
	vector<float> triangleArea;

	/* Defines what is rendered of the terrain*/
	int visibleHeight;
	int visibleWidth;
	int currStartX, currEndX, currStartY, currEndY = 0;

	mutex renderDataMutex;
	atomic_bool reloadGPUData = false;
	vector<GLfloat> renderData; /* Gets sent to GPU */
	vector<unsigned int> indices;
};