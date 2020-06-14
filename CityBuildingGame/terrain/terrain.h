#pragma once

#include <vector>
#include <algorithm>
#include <time.h>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <execution>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Include GLM, math library
#include "glm/glm.hpp"
#include <math.h>
#include "glm/gtc/matrix_transform.hpp"

#include "globals.h"
#include "noise_gen.h"
#include "model.h"
#include "game_object.h"
#include "grid.h"

class Terrain : GameObject
{
public:
	Terrain(int gridHeight, int gridWidth, Grid* aGrid);
	~Terrain();

	void SetRenderWindow(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight);
	void CreateGeometry();
	void Draw(Shader& shader);
	void InitOpenGL();
	void Accept(Visitor& v) override;
	std::vector<std::vector<float>> heightmap;

	/* Defines the size of renderData0, renderData1 (main memory buffers) in terms of tiles
	Change this to allow further zoom out */
	int maximumVisibleUnits = 600 * 500;

	// Set this to force reload of terrain once (for example when going into building mode)
	bool reloadTerrain = false;

private:
	void LoadVisibleGeometry(glm::vec2 upperLeft, glm::vec2 upperRight, glm::vec2 lowerLeft, glm::vec2 lowerRight);
	void ReloadGPUData();

	Grid* grid;
	Model floor_tile;
	GLuint VBO{}, VAO{};
	unsigned int secondary_texture {};
	unsigned int texture_id_grass {};
	unsigned int texture_id_grass_red {};
	unsigned int texture_id_dirt_path{};

	std::string texture_grass = "grass.png";
	std::string texture_grass_red = "grass_red.png";
	std::string texture_dirt_path = "dirt_path.png";

	int gridHeight;
	int gridWidth;

	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::vec3> triangleNormals;
	std::vector<float> triangleArea;

	/* Defines what is rendered of the terrain*/
	int currUpperLeftX = 0;
	int currUpperLeftY = 0;
	int currLowerRightX = 0;
	int currLowerRightY = 0;

	std::mutex renderDataMutex;
	int currRenderData = 1;
	int renderDataVertexCount = 0;
	// if true then we need to copy new data to GPU memory in the next render loop
	bool reloadGPUData = false;
	std::vector<GLfloat>* renderData0 = new std::vector<GLfloat>(maximumVisibleUnits * 54); /* Gets sent to GPU */
	std::vector<GLfloat>* renderData1 = new std::vector<GLfloat>(maximumVisibleUnits * 54); /* .. */

	enum class Texture { Grass, RedGrass, Road};
};
