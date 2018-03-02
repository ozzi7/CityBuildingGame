#include "terrain.h"

Terrain::Terrain() {}

void Terrain::Initialize(int aWidth, int aHeight)
{
	gridHeight = aHeight;
	gridWidth = aWidth;

	Heightmap heightmap_obj;
	heightmap = vector<vector<float>>(gridHeight + 1, vector<float>(gridWidth + 1, 0));
	heightmap_obj.GeneratePerlinNoise(heightmap, gridWidth + 1, gridHeight + 1, 20, 6);

	CreateGrid();
	PopulateGridWithObjects();

	CreateGeometry();
}
void Terrain::InitializeRenderData(int visibleWidth, int visibleHeight)
{
	visibleHeight = visibleHeight;
	visibleWidth = visibleWidth;
	renderData0 = new vector<GLfloat>((visibleHeight + 1)*(visibleWidth + 1)*(48));
	renderData1 = new vector<GLfloat>((visibleHeight + 1)*(visibleWidth + 1)*(48));
}
void Terrain::SetRenderWindow(int startX, int endX, int startY, int endY)
{
	/* Check if terrain must be reloaded to GPU (after scrolling)*/
	if (currStartX != startX || currEndX != endX || currStartY != startY || currEndY != endY)
	{
		currStartX = startX;
		currEndX = endX;
		currStartY = startY;
		currEndY = endY;
		LoadVisibleGeometry(startX, endX, startY, endY);
	}
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
void Terrain::Draw(Shader &shaderTerrain)
{
	// calculate the model matrix for each object and pass it to shader before drawing
	glm::mat4 model = glm::mat4(1.0f);
	shaderTerrain.setMat4("model", model);

	ReloadGPUData();

	// render
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_grass);

	glBindVertexArray(VAO);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, visibleWidth*visibleHeight*6);
}
void Terrain::ReloadGPUData()
{
	renderDataMutex.lock();
	if (reloadGPUData)
	{
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		if(currRenderData)
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (*renderData1).size(), &(*renderData1)[0], GL_STATIC_DRAW);
		else 
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (*renderData0).size(), &(*renderData0)[0], GL_STATIC_DRAW);

		// position attribute, 5th attribute can be 0 for tightly packed, its equal to 3*sizeof(float)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// position attribute, 5th attribute can be 0 for tightly packed, its equal to 3*sizeof(float)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	renderDataMutex.unlock();
}
void Terrain::LoadVisibleGeometry(int startX, int endX, int startY, int endY)
{	
	/* Create geometry data for visible area */
	startX = max(0, startX);
	endX = startX + 250; // min(gridWidth, endX);

	startY = max(0, startY);
	endY = startY + 200; //  min(gridHeight, endY);

	/* Create vertices of triangles */
	visibleHeight = endY - startY;
	visibleWidth = endX - startX;

	///* We could initialize this only once if not implementing zoom and over edge scrolling*/
	vector<GLfloat> *renderDataTemp;
	renderDataMutex.lock();
	if (currRenderData == 1)
	{
		renderDataTemp = renderData0;
	}
	else
	{
		renderDataTemp = renderData1;
	}
	renderDataMutex.unlock();

	/* Load GPU data for visible area */
	float global_i, global_j = 0.0f;
	for (int i = 0; i < visibleHeight; ++i)
	{
		for (int j = 0; j < visibleWidth; ++j)
		{
			global_i = (float)startY + i;
			global_j = (float)startX + j;

			// x/y/z of first vertex
			(*renderDataTemp)[(i*visibleWidth+j)*48] = global_j;
			(*renderDataTemp)[(i*visibleWidth+j)*48+1] = global_i;
			(*renderDataTemp)[(i*visibleWidth+j)*48+2] = heightmap[global_i][global_j];

			// x/y/z of normal vector
			(*renderDataTemp)[(i*visibleWidth + j) * 48+3] = vertexNormals[global_i*(gridWidth + 1) + global_j].x;
			(*renderDataTemp)[(i*visibleWidth + j) * 48+4] = vertexNormals[global_i*(gridWidth + 1) + global_j].y;
			(*renderDataTemp)[(i*visibleWidth + j) * 48+5] = vertexNormals[global_i*(gridWidth + 1) + global_j].z;

			// texture coord X, Y of first vertex
			(*renderDataTemp)[(i*visibleWidth+j)*48+6] = 0.0f;
			(*renderDataTemp)[(i*visibleWidth+j)*48+7] = 0.0f;

			(*renderDataTemp)[(i*visibleWidth+j)*48+8] = global_j + 1;
			(*renderDataTemp)[(i*visibleWidth+j)*48+9] = global_i;
			(*renderDataTemp)[(i*visibleWidth+j)*48+10] = heightmap[global_i][global_j + 1];

			// x/y/z of normal vector
			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 11] = vertexNormals[global_i*(gridWidth + 1) + global_j+1].x;
			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 12] = vertexNormals[global_i*(gridWidth + 1) + global_j+1].y;
			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 13] = vertexNormals[global_i*(gridWidth + 1) + global_j+1].z;

			(*renderDataTemp)[(i*visibleWidth+j)*48+14] = 1.0f;
			(*renderDataTemp)[(i*visibleWidth+j)*48+15] = 0.0f;

			(*renderDataTemp)[(i*visibleWidth+j)*48+16] = global_j;
			(*renderDataTemp)[(i*visibleWidth+j)*48+17] = global_i + 1;
			(*renderDataTemp)[(i*visibleWidth+j)*48+18] = heightmap[global_i + 1][global_j];

			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 19] = vertexNormals[(global_i+1)*(gridWidth + 1) + global_j].x;
			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 20] = vertexNormals[(global_i+1)*(gridWidth + 1) + global_j].y;
			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 21] = vertexNormals[(global_i+1)*(gridWidth + 1) + global_j].z;

			(*renderDataTemp)[(i*visibleWidth+j)*48+22] = 0.0f;
			(*renderDataTemp)[(i*visibleWidth+j)*48+23] = 1.0f;

			(*renderDataTemp)[(i*visibleWidth+j)*48+24] = global_j;
			(*renderDataTemp)[(i*visibleWidth+j)*48+25] = global_i + 1;
			(*renderDataTemp)[(i*visibleWidth+j)*48+26] = heightmap[global_i + 1][global_j];

			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 27] = vertexNormals[(global_i + 1)*(gridWidth + 1) + global_j].x;
			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 28] = vertexNormals[(global_i + 1)*(gridWidth + 1) + global_j].y;
			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 29] = vertexNormals[(global_i + 1)*(gridWidth + 1) + global_j].z;

			(*renderDataTemp)[(i*visibleWidth+j)*48+30] = 0.0f;
			(*renderDataTemp)[(i*visibleWidth+j)*48+31] = 1.0f;

			(*renderDataTemp)[(i*visibleWidth+j)*48+32] = global_j + 1;
			(*renderDataTemp)[(i*visibleWidth+j)*48+33] = global_i;
			(*renderDataTemp)[(i*visibleWidth+j)*48+34] = heightmap[global_i][global_j + 1];

			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 35] = vertexNormals[global_i*(gridWidth + 1) + global_j + 1].x;
			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 36] = vertexNormals[global_i*(gridWidth + 1) + global_j + 1].y;
			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 37] = vertexNormals[global_i*(gridWidth + 1) + global_j + 1].z;

			(*renderDataTemp)[(i*visibleWidth+j)*48+38] = 1.0f;
			(*renderDataTemp)[(i*visibleWidth+j)*48+39] = 0.0f;

			(*renderDataTemp)[(i*visibleWidth+j)*48+40] = global_j + 1;
			(*renderDataTemp)[(i*visibleWidth+j)*48+41] = global_i + 1;
			(*renderDataTemp)[(i*visibleWidth+j)*48+42] = heightmap[global_i + 1][global_j + 1];

			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 43] = vertexNormals[(global_i+1)*(gridWidth + 1) + global_j + 1].x;
			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 44] = vertexNormals[(global_i+1)*(gridWidth + 1) + global_j + 1].y;
			(*renderDataTemp)[(i*visibleWidth + j) * 48 + 45] = vertexNormals[(global_i+1)*(gridWidth + 1) + global_j + 1].z;

			(*renderDataTemp)[(i*visibleWidth+j)*48+46] = 1.0f;
			(*renderDataTemp)[(i*visibleWidth+j)*48+47] = 1.0f;
		}
	}
	renderDataMutex.lock();
	if (currRenderData == 1)
	{
		currRenderData = 0;
	}
	else
	{
		currRenderData = 1;
	}
	reloadGPUData = true;
	renderDataMutex.unlock();
}
void Terrain::CreateGeometry()
{
	/* Create normals, areas for triangles using explicit cross product formula*/
	triangleNormals = vector<glm::vec3>(gridWidth*gridHeight * 2);
	triangleArea = vector<float>(gridWidth*gridHeight * 2);
	for (int i = 0; i < gridHeight; ++i)
	{
		for (int j = 0; j < gridWidth; ++j)
		{
			/* Left triangle */
			glm::vec3 crossProduct = glm::vec3(-(heightmap[i][j + 1] - heightmap[i][j]),
				heightmap[i + 1][j] - heightmap[i][j],
				1);

			triangleArea[(i*gridWidth + j) * 2] = glm::length(crossProduct) / 2.0f;
			triangleNormals[(i*gridWidth + j) * 2] = glm::normalize(crossProduct);

			/* Right triangle*/
			crossProduct = glm::vec3(-(heightmap[i + 1][j + 1] - heightmap[i][j + 1]) -
				(heightmap[i][j + 1] - heightmap[i + 1][j]),
				-(heightmap[i + 1][j + 1] - heightmap[i][j + 1]),
				1);

			triangleArea[(i*gridWidth + j) * 2 + 1] = glm::length(crossProduct) / 2.0f;
			triangleNormals[(i*gridWidth + j) * 2 + 1] = glm::normalize(crossProduct);
		}
	}

	/* Create vertex normals, currently weighted by area of neighbor triangle not angle (should be similar in this case)*/
	vertexNormals = vector<glm::vec3>((gridWidth+1)*(gridHeight+1));
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
}
void Terrain::LoadTextures(Shader & shaderTerrain, string exePath)
{
	Model tree = Model();
	string texturesPath = exePath + "/textures";
	texture_id_grass = tree.TextureFromFile(texture_grass.c_str(), texturesPath);

	shaderTerrain.setInt("texture1", 0);
}
void Terrain::GenerateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
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