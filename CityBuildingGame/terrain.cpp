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
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture_id_grass);

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

	/* Create vertices of triangles */
	visibleHeight = endY - startY;
	visibleWidth = endX - startX;

	// TODO: reuse vertices
	/* 2*3*8: 2 triangles per grid unit, 3 vertices per triangle, each vertex has 3 coordinates, 
	2 texture cooordinates and 3 normal coordinates */
	vector<GLfloat> terrainVector = vector<GLfloat>();
	vector<GLfloat> completeTerrain = vector<GLfloat>();
	vector<unsigned int> indices;

	for(int i = startY; i < endY; ++i)
	{
		for(int j = startX; j < endX; ++j)
		{
			if (i >= startY && i < endY && j >= startX && j < endX)
			{
				// X, Y, Z of first vertex
				terrainVector.push_back(j);
				terrainVector.push_back(i);
				terrainVector.push_back(heightmap[i][j]);

				// normals placeholder
				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);

				// texture coord X, Y of first vertex
				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);

				terrainVector.push_back(j + 1);
				terrainVector.push_back(i);
				terrainVector.push_back(heightmap[i][j + 1]);

				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);

				terrainVector.push_back(1.0f);
				terrainVector.push_back(0.0f);

				terrainVector.push_back(j);
				terrainVector.push_back(i + 1);
				terrainVector.push_back(heightmap[i + 1][j]);

				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);

				terrainVector.push_back(0.0f);
				terrainVector.push_back(1.0f);

				terrainVector.push_back(j);
				terrainVector.push_back(i + 1);
				terrainVector.push_back(heightmap[i + 1][j]);

				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);

				terrainVector.push_back(0.0f);
				terrainVector.push_back(1.0f);

				terrainVector.push_back(j + 1);
				terrainVector.push_back(i);
				terrainVector.push_back(heightmap[i][j + 1]);

				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);

				terrainVector.push_back(1.0f);
				terrainVector.push_back(0.0f);

				terrainVector.push_back(j + 1);
				terrainVector.push_back(i + 1);
				terrainVector.push_back(heightmap[i + 1][j + 1]);

				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);
				terrainVector.push_back(0.0f);

				terrainVector.push_back(1.0f);
				terrainVector.push_back(1.0f);
			}
			// X, Y, Z of first vertex
			completeTerrain.push_back(j);
			completeTerrain.push_back(i);
			completeTerrain.push_back(heightmap[i][j]);

			// normals placeholder
			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);

			// texture coord X, Y of first vertex
			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);

			completeTerrain.push_back(j + 1);
			completeTerrain.push_back(i);
			completeTerrain.push_back(heightmap[i][j + 1]);

			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);

			completeTerrain.push_back(1.0f);
			completeTerrain.push_back(0.0f);

			completeTerrain.push_back(j);
			completeTerrain.push_back(i + 1);
			completeTerrain.push_back(heightmap[i + 1][j]);

			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);

			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(1.0f);

			completeTerrain.push_back(j);
			completeTerrain.push_back(i + 1);
			completeTerrain.push_back(heightmap[i + 1][j]);

			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);

			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(1.0f);

			completeTerrain.push_back(j + 1);
			completeTerrain.push_back(i);
			completeTerrain.push_back(heightmap[i][j + 1]);

			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);

			completeTerrain.push_back(1.0f);
			completeTerrain.push_back(0.0f);

			completeTerrain.push_back(j + 1);
			completeTerrain.push_back(i + 1);
			completeTerrain.push_back(heightmap[i + 1][j + 1]);

			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);
			completeTerrain.push_back(0.0f);

			completeTerrain.push_back(1.0f);
			completeTerrain.push_back(1.0f);
		}
	}

	/* Create indices for vertices*/
	int index = 0;
	for (int i = startY; i < endY; ++i)
	{
		for (int j = startX; j < endX; ++j)
		{
			for (int k = 0; k < 6; ++k)
			{
				indices.push_back(index++);
			}
		}
	}

	/* Create normals for triangles*/
	vector<glm::vec3> triangleNormals = vector<glm::vec3>();
	vector<float> triangleArea = vector<float>();
	for (int i = 0; i < gridHeight; ++i)
	{
		for (int j = 0; j < gridWidth; ++j)
		{
			glm::highp_vec3 crossProduct = glm::cross(glm::vec3(completeTerrain[gridWidth*i * 48+8],
				completeTerrain[gridWidth*i * 48 + j * 48 + 9],
				completeTerrain[gridWidth*i * 48 + j * 48 + 10]) - 
				glm::vec3(completeTerrain[gridWidth*i * 48],
				completeTerrain[gridWidth*i * 48 + j*48 + 1],
				completeTerrain[gridWidth*i * 48 + j * 48+ 2]),
				glm::vec3(completeTerrain[gridWidth*i * 48 + j * 48 + 16],
					completeTerrain[gridWidth*i * 48 + j * 48 + 17],
					completeTerrain[gridWidth*i * 48 + j * 48 + 18]) -
				glm::vec3(completeTerrain[gridWidth*i * 48 + j * 48 + 8],
					completeTerrain[gridWidth*i * 48 + j * 48 + 9],
					completeTerrain[gridWidth*i * 48 + j * 48 + 10]));

			triangleArea.push_back(glm::length(crossProduct)/2.0f);
			triangleNormals.push_back(glm::normalize(crossProduct));

			crossProduct = glm::cross(glm::vec3(completeTerrain[gridWidth*i * 48 + j * 48 + 32],
				completeTerrain[gridWidth*i * 48 + j * 48 + 33],
				completeTerrain[gridWidth*i * 48 + j * 48 + 34])-
				glm::vec3(completeTerrain[gridWidth*i * 48 + j * 48 + 24],
				completeTerrain[gridWidth*i* 48 + j * 48 + 25],
				completeTerrain[gridWidth*i * 48 + j * 48 + 26]),
				glm::vec3(completeTerrain[gridWidth*i * 48 + j * 48 + 40],
					completeTerrain[gridWidth*i * 48 + j * 48 + 41],
					completeTerrain[gridWidth*i * 48 + j * 48 + 42]) -
				glm::vec3(completeTerrain[gridWidth*i * 48 + j * 48 + 24],
					completeTerrain[gridWidth*i * 48 + j * 48 + 25],
					completeTerrain[gridWidth*i * 48 + j * 48 + 26]));

			triangleArea.push_back(glm::length(crossProduct) / 2.0f);
			triangleNormals.push_back(glm::normalize(crossProduct));

			/* for only visible area (startY..endY) and (startX to endX), for simplicity we calculate all for now*/
			/*triangleNormals.push_back(glm::triangleNormal(glm::vec3(terrainVector[visibleWidth*(i - startY) * 48],
				terrainVector[visibleWidth*(i - startY) * 48 + 1],
				terrainVector[visibleWidth*(i - startY) * 48 + 2]),
				glm::vec3(terrainVector[visibleWidth*(i - startY) * 48 + 8],
					terrainVector[visibleWidth*(i - startY) * 48 + 9],
					terrainVector[visibleWidth*(i - startY) * 48 + 10]),
				glm::vec3(terrainVector[visibleWidth*(i - startY) * 48 + 16],
					terrainVector[visibleWidth*(i - startY) * 48 + 17],
					terrainVector[visibleWidth*(i - startY) * 48 + 18])));

			triangleNormals.push_back(glm::triangleNormal(glm::vec3(terrainVector[visibleWidth*(i - startY) * 48 + 24],
				terrainVector[visibleWidth*(i - startY) * 48 + 25],
				terrainVector[visibleWidth*(i - startY) * 48 + 26]),
				glm::vec3(terrainVector[visibleWidth*(i - startY) * 48 + 32],
					terrainVector[visibleWidth*(i - startY) * 48 + 33],
					terrainVector[visibleWidth*(i - startY) * 48 + 34]),
				glm::vec3(terrainVector[visibleWidth*(i - startY) * 48 + 40],
					terrainVector[visibleWidth*(i - startY) * 48 + 41],
					terrainVector[visibleWidth*(i - startY) * 48 + 42])));*/
		}
	}

	/* Create vertex normals, currently weighted by area of neighbor triangle not angle (should be similar in this case)*/
	vector<glm::vec3> vertexNormals = vector<glm::vec3>();
	for (int i = 0; i < gridHeight + 1; ++i)
	{
		for (int j = 0; j < gridWidth + 1; ++j)
		{
			// cycle all vertices
			if (i == 0) {
				// bottom row
				if (j == 0) {
					// bottom left vertex
					vertexNormals.push_back(triangleNormals[0]);
				} else if (j == gridHeight) {
					// bottom right
					vertexNormals.push_back(normalize(triangleArea[(j - 1) * 2] * triangleNormals[(j - 1) * 2] +
						triangleArea[(j - 1) * 2 + 1] * triangleNormals[(j - 1) * 2 + 1]));
				}
				else {
					// bottom middle
					vertexNormals.push_back(normalize(triangleArea[(j - 1) * 2]*triangleNormals[(j-1)*2] +
						triangleArea[(j - 1) * 2 + 1] * triangleNormals[(j - 1) * 2 + 1])+
						triangleArea[(j - 1) * 2 + 2] * triangleNormals[(j - 1) * 2 + 2]);
				}
			}
			else {
				if (j == 0 && i < gridHeight) {
					// left column, except bottomn left and top left
					vertexNormals.push_back(normalize(triangleArea[(i-1)*gridWidth*2] * triangleNormals[(i - 1)*gridWidth * 2] +
						triangleArea[(i - 1)*gridWidth * 2+1] * triangleNormals[(i - 1)*gridWidth * 2+1] +
						triangleArea[i*gridWidth * 2] * triangleNormals[i * gridWidth*2]));
				}
				else if(j == 0) {
					// top left
					vertexNormals.push_back(normalize(triangleArea[(i - 1)*gridWidth * 2] * triangleNormals[(i - 1)*gridWidth * 2] +
						triangleArea[(i - 1)*gridWidth * 2 + 1] * triangleNormals[(i - 1)*gridWidth * 2 + 1]));
				}
				else if (j == gridWidth && i != gridHeight) {
					// right side, middle 
					vertexNormals.push_back(normalize(triangleArea[(i - 1)*gridWidth * 2 +j*2 - 1] * triangleNormals[(i - 1)*gridWidth * 2 + j * 2 - 1] +
						triangleArea[i *gridWidth * 2 + j * 2 - 2] * triangleNormals[i *gridWidth * 2 + j * 2 - 2] +
						triangleArea[i *gridWidth * 2 + j * 2 - 1] * triangleNormals[i *gridWidth * 2 + j * 2 - 1]));
				}
				else if (j == gridWidth && i == gridHeight) {
					// right top 
					vertexNormals.push_back(triangleNormals[gridWidth*gridHeight-1]);
				}
				else if (i == gridHeight) {
					// top row, except left and right 
					vertexNormals.push_back(normalize(triangleArea[(i - 1)*gridWidth * 2 + (j*2)-1] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2) - 1] +
						triangleArea[(i - 1)*gridWidth * 2 + (j * 2)] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2)] +
						triangleArea[(i - 1)*gridWidth * 2 + (j * 2)+1] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2) + 1]));
				}
				else
				{
					// rest, all in the middle with 6 surrounding triangles
					vertexNormals.push_back(normalize(triangleArea[(i - 1)*gridWidth * 2 + (j * 2) - 1] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2) - 1] +
						triangleArea[(i - 1)*gridWidth * 2 + (j * 2)] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2)] +
						triangleArea[(i - 1)*gridWidth * 2 + (j * 2) + 1] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2) + 1]+
						triangleArea[i*gridWidth * 2 + (j * 2) - 2] * triangleNormals[i*gridWidth * 2 + (j * 2) - 2] +
						triangleArea[i*gridWidth * 2 + (j * 2) - 1] * triangleNormals[i*gridWidth * 2 + (j * 2) - 1] +
						triangleArea[i*gridWidth * 2 + (j * 2)]  * triangleNormals[i*gridWidth * 2 + (j * 2) + 1]));
				}
			}

		}
	}
	for (int i = startY; i < endY; ++i)
	{
		for (int j = startX; j < endX; ++j)
		{
			int globalY = i - startY;
			int globalX = j - startX;
			
			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 3] = vertexNormals[globalY*(gridWidth + 1) + globalX].x;
			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 4] = vertexNormals[globalY*(gridWidth + 1) + globalX].y;
			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 5] = vertexNormals[globalY*(gridWidth + 1) + globalX].z;

			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 11] = vertexNormals[globalY*(gridWidth + 1) + globalX+1].x;
			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 12] = vertexNormals[globalY*(gridWidth + 1) + globalX+1].y;
			terrainVector[visibleWidth*globalY* 48 + globalX * 48 + 13] = vertexNormals[globalY*(gridWidth + 1) + globalX+1].z;

			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 19] = vertexNormals[(globalY+1)*(gridWidth + 1) + globalX].x;
			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 20] = vertexNormals[(globalY+1)*(gridWidth + 1) + globalX].y;
			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 21] = vertexNormals[(globalY+1)*(gridWidth + 1) + globalX].z;

			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 27] = vertexNormals[(globalY + 1)*(gridWidth + 1) + globalX].x;
			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 28] = vertexNormals[(globalY + 1)*(gridWidth + 1) + globalX].y;
			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 29] = vertexNormals[(globalY + 1)*(gridWidth + 1) + globalX].z;

			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 35] = vertexNormals[globalY*(gridWidth + 1) + globalX + 1].x;
			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 36] = vertexNormals[globalY*(gridWidth + 1) + globalX + 1].y;
			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 37] = vertexNormals[globalY*(gridWidth + 1) + globalX + 1].z;

			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 43] = vertexNormals[(globalY + 1)*(gridWidth + 1) + globalX+1].x;
			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 44] = vertexNormals[(globalY + 1)*(gridWidth + 1) + globalX+1].y;
			terrainVector[visibleWidth*globalY * 48 + globalX * 48 + 45] = vertexNormals[(globalY + 1)*(gridWidth + 1) + globalX+1].z;
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// position attribute, 5th attribute can be 0 for tightly packed, its equal to 3*sizeof(float)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
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