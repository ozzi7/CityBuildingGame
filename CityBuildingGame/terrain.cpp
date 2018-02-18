#include "terrain.h"
#include <string>
#include <algorithm>

// Include GLFW, implements openGL
#include <GLFW/glfw3.h>
#include "shader.h"
#include "stb_image.h"

using namespace std;

Terrain::Terrain() {}

void Terrain::Initialize(int aWidth, int aHeight)
{
	gridHeight = aHeight;
	gridWidth = aWidth;

	heightmap = vector<vector<float>>(gridHeight+1, vector<float>(gridWidth+1, 0));
	Heightmap heightmap_obj;
	heightmap_obj.GeneratePerlinNoise(heightmap, gridWidth+1, gridHeight+1, 20, 6);

	CreateGrid();
	AddTexturesToGrid();
	PopulateGridWithObjects();

	LoadTextures();
	CreateGeometry();
}
float Terrain::GetHeight(int argX, int argY)
{
	return grid[argY][argX].averageHeight;
}
void Terrain::CreateGrid()
{
	vector<vector<Unit>> grid = vector<vector<Unit>>(gridHeight, vector<Unit>(gridWidth, 0.0f));
	for (int i = 0; i < heightmap.size() - 1 ; ++i) {
		for (int j = 0; j < heightmap[i].size() - 1; ++j) {
			grid[i][j] = Unit((heightmap[i+1][j] + heightmap[i][j + 1])/2.0f);
		}
	}
}
void Terrain::AddTexturesToGrid()
{

}
void Terrain::PopulateGridWithObjects()
{

}
void Terrain::Draw()
{
	// render
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_grass);

	glBindVertexArray(VAO);

	// calculate the model matrix for each object and pass it to shader before drawing
	glm::mat4 model = glm::mat4(1.0f);
	ourShader.setMat4("model", model);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, visibleWidth*visibleHeight*6);
}
void Terrain::CreateGeometry()
{
	Terrain::CreateGeometry(0, gridWidth, 0, gridHeight);
}
void Terrain::CreateGeometry(int startX, int endX, int startY, int endY)
{
	startX = max(0, startX);
	endX = min(gridWidth, endX);

	startY = max(0, startY);
	endY = min(gridHeight, endY);

	vector<GLfloat> terrainVector;
	vector<unsigned int> indices;

	int index = 0;
	visibleHeight = endY - startY;
	visibleWidth = endX - startX;
	for(int i = startY; i < endY; ++i)
	{
		for(int j = startX; j < endX; ++j)
		{
			// X, Y, Z
			terrainVector.push_back(j);
			terrainVector.push_back(i); 
			terrainVector.push_back(heightmap[i][j]);

			// texture coord X, Y 
			terrainVector.push_back(0.0f);
			terrainVector.push_back(0.0f);

			terrainVector.push_back(j + 1);
			terrainVector.push_back(i);
			terrainVector.push_back(heightmap[i][j + 1]);

			terrainVector.push_back(1.0f);
			terrainVector.push_back(0.0f);

			terrainVector.push_back(j);
			terrainVector.push_back(i + 1);
			terrainVector.push_back(heightmap[i+1][j]);

			terrainVector.push_back(0.0f);
			terrainVector.push_back(1.0f);

			terrainVector.push_back(j); 
			terrainVector.push_back(i + 1); 
			terrainVector.push_back(heightmap[i+1][j]);

			terrainVector.push_back(0.0f);
			terrainVector.push_back(1.0f);

			terrainVector.push_back(j + 1);
			terrainVector.push_back(i);
			terrainVector.push_back(heightmap[i][j+1]);

			terrainVector.push_back(1.0f);
			terrainVector.push_back(0.0f);

			terrainVector.push_back(j+1);
			terrainVector.push_back(i+1);
			terrainVector.push_back(heightmap[i + 1][j + 1]);

			terrainVector.push_back(1.0f);
			terrainVector.push_back(1.0f);

			indices.push_back(index); index++;
			indices.push_back(index); index++;
			indices.push_back(index); index++;
			indices.push_back(index); index++;
			indices.push_back(index); index++;
			indices.push_back(index); index++;
		}
	}

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * terrainVector.size(), &terrainVector[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	// position attribute, 5th attribute can be 0 for tightly packed, its equal to 3*sizeof(float)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Terrain::LoadTextures()
{
	texture_id_grass = Common::loadTexture(texture_grass);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------

	ourShader.setInt("texture1", 0);
}
Terrain::~Terrain()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
Unit::Unit(float pAverageHeight)
{
	averageHeight = pAverageHeight;
}
Unit::~Unit()
{

}