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

	Heightmap heightmap_obj;
	heightmap = vector<vector<float>>(gridHeight+1, vector<float>(gridWidth+1, 0));
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
void Terrain::LoadVisibleGeometry()
{
	LoadVisibleGeometry(0, gridWidth, 0, gridHeight);
}
void Terrain::LoadVisibleGeometry(int startX, int endX, int startY, int endY)
{
	startX = max(0, startX);
	endX = min(gridWidth, endX);

	startY = max(0, startY);
	endY = min(gridHeight, endY);

	/* Create vertices of triangles */
	visibleHeight = endY - startY;
	visibleWidth = endX - startX;
}
void Terrain::CreateGeometry()
{
	vector<GLfloat> renderData = vector<GLfloat>();
	vector<unsigned int> indices;

	for(int i = 0; i < gridHeight; ++i)
	{
		for(int j = 0; j < gridWidth; ++j)
		{
			// X, Y, Z of first vertex
			renderData.push_back(j);
			renderData.push_back(i);
			renderData.push_back(heightmap[i][j]);

			// normals placeholder
			renderData.push_back(0.0f);
			renderData.push_back(0.0f);
			renderData.push_back(0.0f);

			// texture coord X, Y of first vertex
			renderData.push_back(0.0f);
			renderData.push_back(0.0f);

			renderData.push_back(j + 1);
			renderData.push_back(i);
			renderData.push_back(heightmap[i][j + 1]);

			renderData.push_back(0.0f);
			renderData.push_back(0.0f);
			renderData.push_back(0.0f);

			renderData.push_back(1.0f);
			renderData.push_back(0.0f);

			renderData.push_back(j);
			renderData.push_back(i + 1);
			renderData.push_back(heightmap[i + 1][j]);

			renderData.push_back(0.0f);
			renderData.push_back(0.0f);
			renderData.push_back(0.0f);

			renderData.push_back(0.0f);
			renderData.push_back(1.0f);

			renderData.push_back(j);
			renderData.push_back(i + 1);
			renderData.push_back(heightmap[i + 1][j]);

			renderData.push_back(0.0f);
			renderData.push_back(0.0f);
			renderData.push_back(0.0f);

			renderData.push_back(0.0f);
			renderData.push_back(1.0f);

			renderData.push_back(j + 1);
			renderData.push_back(i);
			renderData.push_back(heightmap[i][j + 1]);

			renderData.push_back(0.0f);
			renderData.push_back(0.0f);
			renderData.push_back(0.0f);

			renderData.push_back(1.0f);
			renderData.push_back(0.0f);

			renderData.push_back(j + 1);
			renderData.push_back(i + 1);
			renderData.push_back(heightmap[i + 1][j + 1]);

			renderData.push_back(0.0f);
			renderData.push_back(0.0f);
			renderData.push_back(0.0f);

			renderData.push_back(1.0f);
			renderData.push_back(1.0f);
		}
	}

	/* Create indices for vertices*/
	int index = 0;
	for (int i = 0; i < gridHeight; ++i)
	{
		for (int j = 0; j < gridWidth; ++j)
		{
			for (int k = 0; k < 6; ++k)
			{
				indices.push_back(index++);
			}
		}
	}

	/* Create normals, areas for triangles using explicit cross product formula*/
	vector<glm::vec3> triangleNormals = vector<glm::vec3>(gridWidth*gridHeight * 2);
	vector<float> triangleArea = vector<float>(gridWidth*gridHeight * 2);
	for (int i = 0; i < gridHeight; ++i)
	{
		for (int j = 0; j < gridWidth; ++j)
		{
			/* Left triangle */
			glm::highp_vec3 crossProduct = glm::highp_vec3(-(heightmap[i][j + 1] - heightmap[i][j]),
				heightmap[i + 1][j] - heightmap[i][j],
				1);

			triangleArea[(i*gridWidth + j) * 2] = glm::length(crossProduct) / 2.0f;
			triangleNormals[(i*gridWidth + j) * 2] = glm::normalize(crossProduct);

			/* Right triangle*/
			crossProduct = glm::highp_vec3(-(heightmap[i + 1][j + 1] - heightmap[i][j + 1]) -
				(heightmap[i][j + 1] - heightmap[i + 1][j]),
				-(heightmap[i + 1][j + 1] - heightmap[i][j + 1]),
				1);

			triangleArea[(i*gridWidth + j) * 2 + 1] = glm::length(crossProduct) / 2.0f;
			triangleNormals[(i*gridWidth + j) * 2 + 1] = glm::normalize(crossProduct);
		}
	}

	/* Create vertex normals, currently weighted by area of neighbor triangle not angle (should be similar in this case)*/
	vector<glm::vec3> vertexNormals = vector<glm::vec3>((gridWidth+1)*(gridHeight+1));
	for (int i = 0; i < gridHeight + 1; ++i)
	{
		for (int j = 0; j < gridWidth + 1; ++j)
		{
			// cycle all vertices
			if (i == 0) {
				// bottom row
				if (j == 0) {
					// bottom left vertex
					vertexNormals[i*(gridWidth+1)+j] = triangleNormals[0];
				} else if (j == gridHeight) {
					// bottom right
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(j - 1) * 2] * triangleNormals[(j - 1) * 2] +
						triangleArea[(j - 1) * 2 + 1] * triangleNormals[(j - 1) * 2 + 1]);
				}
				else {
					// bottom middle
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(j - 1) * 2]*triangleNormals[(j-1)*2] +
						triangleArea[(j - 1) * 2 + 1] * triangleNormals[(j - 1) * 2 + 1])+
						triangleArea[(j - 1) * 2 + 2] * triangleNormals[(j - 1) * 2 + 2];
				}
			}
			else {
				if (j == 0 && i < gridHeight) {
					// left column, except bottomn left and top left
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(i-1)*gridWidth*2] * triangleNormals[(i - 1)*gridWidth * 2] +
						triangleArea[(i - 1)*gridWidth * 2+1] * triangleNormals[(i - 1)*gridWidth * 2+1] +
						triangleArea[i*gridWidth * 2] * triangleNormals[i * gridWidth*2]);
				}
				else if(j == 0) {
					// top left
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(i - 1)*gridWidth * 2] * triangleNormals[(i - 1)*gridWidth * 2] +
						triangleArea[(i - 1)*gridWidth * 2 + 1] * triangleNormals[(i - 1)*gridWidth * 2 + 1]);
				}
				else if (j == gridWidth && i != gridHeight) {
					// right side, middle 
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(i - 1)*gridWidth * 2 +j*2 - 1] * triangleNormals[(i - 1)*gridWidth * 2 + j * 2 - 1] +
						triangleArea[i *gridWidth * 2 + j * 2 - 2] * triangleNormals[i *gridWidth * 2 + j * 2 - 2] +
						triangleArea[i *gridWidth * 2 + j * 2 - 1] * triangleNormals[i *gridWidth * 2 + j * 2 - 1]);
				}
				else if (j == gridWidth && i == gridHeight) {
					// right top 
					vertexNormals[i*(gridWidth + 1) + j] = triangleNormals[gridWidth*gridHeight-1];
				}
				else if (i == gridHeight) {
					// top row, except left and right 
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(i - 1)*gridWidth * 2 + (j*2)-1] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2) - 1] +
						triangleArea[(i - 1)*gridWidth * 2 + (j * 2)] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2)] +
						triangleArea[(i - 1)*gridWidth * 2 + (j * 2)+1] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2) + 1]);
				}
				else
				{
					// rest, all in the middle with 6 surrounding triangles
					vertexNormals[i*(gridWidth + 1) + j] = normalize(triangleArea[(i - 1)*gridWidth * 2 + (j * 2) - 1] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2) - 1] +
						triangleArea[(i - 1)*gridWidth * 2 + (j * 2)] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2)] +
						triangleArea[(i - 1)*gridWidth * 2 + (j * 2) + 1] * triangleNormals[(i - 1)*gridWidth * 2 + (j * 2) + 1]+
						triangleArea[i*gridWidth * 2 + (j * 2) - 2] * triangleNormals[i*gridWidth * 2 + (j * 2) - 2] +
						triangleArea[i*gridWidth * 2 + (j * 2) - 1] * triangleNormals[i*gridWidth * 2 + (j * 2) - 1] +
						triangleArea[i*gridWidth * 2 + (j * 2)]  * triangleNormals[i*gridWidth * 2 + (j * 2) + 1]);
				}
			}

		}
	}
	visibleWidth = gridWidth;
	visibleHeight = gridHeight;
	for (int i = 0; i < gridHeight; ++i)
	{
		for (int j = 0; j < gridWidth; ++j)
		{
			int globalY = i - 0;
			int globalX = j - 0;
			
			renderData[visibleWidth*globalY * 48 + globalX * 48 + 3] = vertexNormals[globalY*(gridWidth + 1) + globalX].x;
			renderData[visibleWidth*globalY * 48 + globalX * 48 + 4] = vertexNormals[globalY*(gridWidth + 1) + globalX].y;
			renderData[visibleWidth*globalY * 48 + globalX * 48 + 5] = vertexNormals[globalY*(gridWidth + 1) + globalX].z;

			renderData[visibleWidth*globalY * 48 + globalX * 48 + 11] = vertexNormals[globalY*(gridWidth + 1) + globalX+1].x;
			renderData[visibleWidth*globalY * 48 + globalX * 48 + 12] = vertexNormals[globalY*(gridWidth + 1) + globalX+1].y;
			renderData[visibleWidth*globalY* 48 + globalX * 48 + 13] = vertexNormals[globalY*(gridWidth + 1) + globalX+1].z;

			renderData[visibleWidth*globalY * 48 + globalX * 48 + 19] = vertexNormals[(globalY+1)*(gridWidth + 1) + globalX].x;
			renderData[visibleWidth*globalY * 48 + globalX * 48 + 20] = vertexNormals[(globalY+1)*(gridWidth + 1) + globalX].y;
			renderData[visibleWidth*globalY * 48 + globalX * 48 + 21] = vertexNormals[(globalY+1)*(gridWidth + 1) + globalX].z;

			renderData[visibleWidth*globalY * 48 + globalX * 48 + 27] = vertexNormals[(globalY + 1)*(gridWidth + 1) + globalX].x;
			renderData[visibleWidth*globalY * 48 + globalX * 48 + 28] = vertexNormals[(globalY + 1)*(gridWidth + 1) + globalX].y;
			renderData[visibleWidth*globalY * 48 + globalX * 48 + 29] = vertexNormals[(globalY + 1)*(gridWidth + 1) + globalX].z;

			renderData[visibleWidth*globalY * 48 + globalX * 48 + 35] = vertexNormals[globalY*(gridWidth + 1) + globalX + 1].x;
			renderData[visibleWidth*globalY * 48 + globalX * 48 + 36] = vertexNormals[globalY*(gridWidth + 1) + globalX + 1].y;
			renderData[visibleWidth*globalY * 48 + globalX * 48 + 37] = vertexNormals[globalY*(gridWidth + 1) + globalX + 1].z;

			renderData[visibleWidth*globalY * 48 + globalX * 48 + 43] = vertexNormals[(globalY + 1)*(gridWidth + 1) + globalX+1].x;
			renderData[visibleWidth*globalY * 48 + globalX * 48 + 44] = vertexNormals[(globalY + 1)*(gridWidth + 1) + globalX+1].y;
			renderData[visibleWidth*globalY * 48 + globalX * 48 + 45] = vertexNormals[(globalY + 1)*(gridWidth + 1) + globalX+1].z;
		}
	}
			
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderData.size(), &renderData[0], GL_STATIC_DRAW);

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